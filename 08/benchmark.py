import json
import time
import cjson
import ujson

with open('dataset.json', 'r') as f:
    data_array = json.load(f)

print(f"Загружено {len(data_array)} объектов")

total_loads_times = {"json": 0, "ujson": 0, "cjson": 0}
total_dumps_times = {"json": 0, "ujson": 0, "cjson": 0}

for obj in data_array:
    compatible_obj = {}
    for key, value in obj.items():
        if isinstance(value, bool):
            compatible_obj[key] = 1 if value else 0
        elif isinstance(value, str):
            try:
                if '.' in value:
                    compatible_obj[key] = float(value)
                else:
                    compatible_obj[key] = int(value)
            except ValueError:
                compatible_obj[key] = value
        elif isinstance(value, (int, float)):
            compatible_obj[key] = value
    
    json_str = json.dumps(compatible_obj, ensure_ascii=False)
    
    
    iterations = 100

    # Тест loads
    
    # json
    start = time.time()
    for _ in range(iterations):
        json.loads(json_str)
    json_time = time.time() - start
    total_loads_times["json"] += json_time
    
    # ujson
    start = time.time()
    for _ in range(iterations):
        ujson.loads(json_str)
    ujson_time = time.time() - start
    total_loads_times["ujson"] += ujson_time
    
    # cjson
    start = time.time()
    for _ in range(iterations):
        cjson.loads(json_str)
    cjson_time = time.time() - start
    total_loads_times["cjson"] += cjson_time
    
    # Тест dumps
    
    # json
    start = time.time()
    for _ in range(iterations):
        json.dumps(compatible_obj)
    json_time = time.time() - start
    total_dumps_times["json"] += json_time
    
    # ujson
    start = time.time()
    for _ in range(iterations):
        ujson.dumps(compatible_obj)
    ujson_time = time.time() - start
    total_dumps_times["ujson"] += ujson_time
    
    # cjson
    start = time.time()
    for _ in range(iterations):
        cjson.dumps(compatible_obj)
    cjson_time = time.time() - start
    total_dumps_times["cjson"] += cjson_time

print(f"\nПротестировано объектов: {len(data_array)}")
print(f"Итераций на каждый объект: {iterations}")
print(f"Всего операций loads: {len(data_array) * iterations}")
print(f"Всего операций dumps: {len(data_array) * iterations}")

print("\nСуммарное время loads:")
print(f"  json.loads : {total_loads_times['json']:.4f} сек")
print(f"  ujson.loads: {total_loads_times['ujson']:.4f} сек")
print(f"  cjson.loads: {total_loads_times['cjson']:.4f} сек")

print(f"\nПроизводительность loads относительно обычного json:")
json_ref = total_loads_times['json']
print(f"  json.loads  : 1.0x (базис)")
print(f"  ujson.loads : {json_ref/total_loads_times['ujson']:.1f}x")
print(f"  cjson.loads : {json_ref/total_loads_times['cjson']:.1f}x")

print("\nСуммарное время dumps:")
print(f"  json.dumps : {total_dumps_times['json']:.4f} сек")
print(f"  ujson.dumps: {total_dumps_times['ujson']:.4f} сек")
print(f"  cjson.dumps: {total_dumps_times['cjson']:.4f} сек")

print(f"\nПроизводительность dumps относительно обычного json:")
json_ref = total_dumps_times['json']
print(f"  json.dumps  : 1.0x (базис)")
print(f"  ujson.dumps : {json_ref/total_dumps_times['ujson']:.1f}x")
print(f"  cjson.dumps : {json_ref/total_dumps_times['cjson']:.1f}x")