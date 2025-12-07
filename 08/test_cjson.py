import json
import unittest
import cjson

class TestCJson(unittest.TestCase):
    def test_simple_dict(self):
        """Тестирование базового случая"""
        json_str = '{"hello": 10, "world": "value"}'
        json_doc = json.loads(json_str)
        cjson_doc = cjson.loads(json_str)
        
        self.assertEqual(json_doc, cjson_doc)
        self.assertEqual(json_str, cjson.dumps(cjson.loads(json_str)))
    
    def test_empty_dict(self):
        """Тестирование пустого словаря"""
        json_str = '{}'
        json_doc = json.loads(json_str)
        cjson_doc = cjson.loads(json_str)
        
        self.assertEqual(json_doc, cjson_doc)
        self.assertEqual(json_str, cjson.dumps(cjson.loads(json_str)))
    
    def test_single_item(self):
        """Тестирование словаря с одним элементом"""
        json_str = '{"key": "value"}'
        json_doc = json.loads(json_str)
        cjson_doc = cjson.loads(json_str)
        
        self.assertEqual(json_doc, cjson_doc)
        self.assertEqual(json_str, cjson.dumps(cjson.loads(json_str)))
    
    def test_number_types(self):
        """Тестирование различных числовых типов"""
        test_cases = [
            '{"int": 42}',
            '{"negative": -10}',
            '{"float": 3.14}',
            '{"zero": 0}',
        ]
        
        for json_str in test_cases:
            with self.subTest(json_str=json_str):
                json_doc = json.loads(json_str)
                cjson_doc = cjson.loads(json_str)
                for key in json_doc:
                    if isinstance(json_doc[key], float):
                        self.assertAlmostEqual(json_doc[key], cjson_doc[key], places=5)
                    else:
                        self.assertEqual(json_doc[key], cjson_doc[key])
                
                cjson_str = cjson.dumps(cjson_doc)
                self.assertEqual(json.loads(json_str), cjson.loads(cjson_str))
    
    def test_invalid_json(self):
        """Тестирование обработки некорректного json"""
        invalid_cases = [
            '{"key": value}',  # значение без кавычек
            '{"key": }',       # нет значения
            '{key: "value"}',  # ключ без кавычек
            '{"key": "value"', # нет закрывающей скобки
            '',                # пустая строка
            '{',               # неполный объект
        ]
        
        for invalid_json in invalid_cases:
            with self.subTest(invalid_json=invalid_json):
                with self.assertRaises(Exception):
                    cjson.loads(invalid_json)
    
    def test_dumps_with_various_types(self):
        """Тестирование сериализации (dumps) различных типов данных"""
        test_dicts = [
            {"name": "Alice", "age": 30},
            {"temperature": -5.5, "city": "Moscow"},
            {"empty": "", "number": 0},
        ]
        
        for test_dict in test_dicts:
            with self.subTest(test_dict=test_dict):
                json_str = cjson.dumps(test_dict)
                parsed = cjson.loads(json_str)
                self.assertEqual(test_dict, parsed)

    def test_unicode_strings(self):
        """Тестирование Unicode-строк"""
        json_str = '{"name": "Иван", "city": "Москва"}'
        json_doc = json.loads(json_str)
        cjson_doc = cjson.loads(json_str)
        self.assertEqual(json_doc, cjson_doc)

def main():
    """Главная функция для запуска тестов"""
    unittest.main(argv=[''], verbosity=2, exit=False)

if __name__ == "__main__":
    main()