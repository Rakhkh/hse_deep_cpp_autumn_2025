#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Структура для парсера
typedef struct {
    const char *str;
    size_t pos;
    size_t len;
} JsonParser;

// Функции для работы с парсером
static void skip_whitespace(JsonParser *parser) {
    while (parser->pos < parser->len && isspace(parser->str[parser->pos])) 
        parser->pos++;
}

static int expect_char(JsonParser *parser, char expected) {
    skip_whitespace(parser);
    if (parser->pos >= parser->len || parser->str[parser->pos] != expected)
        return 0;
    parser->pos++;
    return 1;
}

static PyObject *parse_string(JsonParser *parser) {
    if (!expect_char(parser, '"')) 
        return NULL;
    
    size_t start = parser->pos;
    while (parser->pos < parser->len && parser->str[parser->pos] != '"') 
        parser->pos++;
    
    if (parser->pos >= parser->len) {
        PyErr_SetString(PyExc_ValueError, "Unterminated string");
        return NULL;
    }
    
    size_t length = parser->pos - start;
    PyObject *str = PyUnicode_FromStringAndSize(&parser->str[start], length);
    parser->pos++; 
    
    return str;
}

static PyObject *parse_number(JsonParser *parser) {
    skip_whitespace(parser);
    
    size_t start = parser->pos;
    int is_float = 0;
    
    if (parser->str[parser->pos] == '-') 
        parser->pos++;
    
    while (parser->pos < parser->len && isdigit(parser->str[parser->pos])) 
        parser->pos++;
    
    if (parser->pos < parser->len && parser->str[parser->pos] == '.') {
        is_float = 1;
        parser->pos++;
        while (parser->pos < parser->len && isdigit(parser->str[parser->pos])) 
            parser->pos++;
    }
    
    size_t length = parser->pos - start;
    if (length == 0) 
        return NULL;
    
    char *num_str = malloc(length + 1);
    if (!num_str) {
        PyErr_NoMemory();
        return NULL;
    }
    
    strncpy(num_str, &parser->str[start], length);
    num_str[length] = '\0';
    
    PyObject *num;
    if (is_float) {
        double value = atof(num_str);
        num = PyFloat_FromDouble(value);
    } 
    else {
        long value = atol(num_str);
        num = PyLong_FromLong(value);
    }
    
    free(num_str);
    return num;
}

static PyObject *parse_value(JsonParser *parser) {
    skip_whitespace(parser);
    
    if (parser->pos >= parser->len) 
        return NULL;
    
    char c = parser->str[parser->pos];
    
    if (c == '"') 
        return parse_string(parser);
    else if (isdigit(c) || c == '-') 
        return parse_number(parser);
    
    PyErr_SetString(PyExc_ValueError, "Unexpected character in value");
    return NULL;
}

// Основная функция loads
static PyObject *cjson_loads(PyObject *self, PyObject *args) {
    const char *json_str;
    
    if (!PyArg_ParseTuple(args, "s", &json_str)) 
        return NULL;
    
    JsonParser parser = {
        .str = json_str,
        .pos = 0,
        .len = strlen(json_str)
    };
    
    skip_whitespace(&parser);
    
    if (!expect_char(&parser, '{')) {
        PyErr_SetString(PyExc_ValueError, "json must start with '{'");
        return NULL;
    }
    
    PyObject *dict = PyDict_New();
    if (!dict) 
        return NULL;
    
    skip_whitespace(&parser);
    
    if (parser.str[parser.pos] == '}') {
        parser.pos++;
        return dict;
    }
    
    while (1) {
        PyObject *key = parse_string(&parser);
        if (!key) {
            Py_DECREF(dict);
            return NULL;
        }
        
        if (!expect_char(&parser, ':')) {
            Py_DECREF(key);
            Py_DECREF(dict);
            PyErr_SetString(PyExc_ValueError, "Expected ':' after key");
            return NULL;
        }
        
        PyObject *value = parse_value(&parser);
        if (!value) {
            Py_DECREF(key);
            Py_DECREF(dict);
            return NULL;
        }
        
        if (PyDict_SetItem(dict, key, value) < 0) {
            Py_DECREF(key);
            Py_DECREF(value);
            Py_DECREF(dict);
            return NULL;
        }
        
        Py_DECREF(key);
        Py_DECREF(value);
        
        skip_whitespace(&parser);
        
        if (parser.pos >= parser.len) {
            PyErr_SetString(PyExc_ValueError, "Unexpected end of json");
            Py_DECREF(dict);
            return NULL;
        }
        
        if (parser.str[parser.pos] == '}') {
            parser.pos++;
            break;
        }
        
        if (parser.str[parser.pos] != ',') {
            PyErr_SetString(PyExc_ValueError, "Expected ',' or '}'");
            Py_DECREF(dict);
            return NULL;
        }
        
        parser.pos++; 
        skip_whitespace(&parser);
    }
    
    return dict;
}

// Основная функция dumps
static PyObject *cjson_dumps(PyObject *self, PyObject *args) {
    PyObject *dict;
    
    if (!PyArg_ParseTuple(args, "O", &dict)) 
        return NULL;
    
    if (!PyDict_Check(dict)) {
        PyErr_SetString(PyExc_TypeError, "Argument must be a dictionary");
        return NULL;
    }
    
    PyObject *items = PyDict_Items(dict);
    if (!items) 
        return NULL;
    
    Py_ssize_t size = PyList_Size(items);
    PyObject *result_str = PyUnicode_FromString("{");
    
    for (Py_ssize_t i = 0; i < size; i++) {
        PyObject *item = PyList_GetItem(items, i);
        PyObject *key = PyTuple_GetItem(item, 0);
        PyObject *value = PyTuple_GetItem(item, 1);
        
        if (i > 0) {
            PyObject *comma = PyUnicode_FromString(", ");
            Py_SETREF(result_str, PyUnicode_Concat(result_str, comma));
            Py_DECREF(comma);
        }
        
        const char *key_str = PyUnicode_AsUTF8(key);
        PyObject *quoted_key = PyUnicode_FromFormat("\"%s\"", key_str);
        Py_SETREF(result_str, PyUnicode_Concat(result_str, quoted_key));
        Py_DECREF(quoted_key);
        
        PyObject *colon = PyUnicode_FromString(": ");
        Py_SETREF(result_str, PyUnicode_Concat(result_str, colon));
        Py_DECREF(colon);
        
        if (PyLong_Check(value)) {
            PyObject *value_str = PyObject_Str(value);
            Py_SETREF(result_str, PyUnicode_Concat(result_str, value_str));
            Py_DECREF(value_str);
        } else if (PyFloat_Check(value)) {
            PyObject *value_str = PyObject_Str(value);
            Py_SETREF(result_str, PyUnicode_Concat(result_str, value_str));
            Py_DECREF(value_str);
        } else if (PyUnicode_Check(value)) {
            const char *value_str = PyUnicode_AsUTF8(value);
            PyObject *quoted_value = PyUnicode_FromFormat("\"%s\"", value_str);
            Py_SETREF(result_str, PyUnicode_Concat(result_str, quoted_value));
            Py_DECREF(quoted_value);
        } else {
            Py_DECREF(result_str);
            Py_DECREF(items);
            PyErr_SetString(PyExc_TypeError, "Unsupported value type");
            return NULL;
        }
    }
    
    Py_DECREF(items);
    
    PyObject *closing = PyUnicode_FromString("}");
    Py_SETREF(result_str, PyUnicode_Concat(result_str, closing));
    Py_DECREF(closing);
    
    return result_str;
}

// Определение методов модуля
static PyMethodDef CJsonMethods[] = {
    {"loads", cjson_loads, METH_VARARGS, "Parse a json string."},
    {"dumps", cjson_dumps, METH_VARARGS, "Serialize a dictionary to json string."},
    {NULL, NULL, 0, NULL}
};

// Определение модуля
static struct PyModuleDef cjsonmodule = {
    PyModuleDef_HEAD_INIT,
    "cjson",
    NULL,
    -1,
    CJsonMethods
};

// Инициализация модуля
PyMODINIT_FUNC PyInit_cjson(void) {
    return PyModule_Create(&cjsonmodule);
}