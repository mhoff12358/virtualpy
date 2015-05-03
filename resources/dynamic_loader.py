
import importlib, sys

def import_as_func(module_path, function_name = "update"):
    existing_module = sys.modules.pop(module_path, None)
    new_module = importlib.import_module(module_path)
    if not existing_module is None:
        sys.modules[module_path] = existing_module
    return new_module.__getattribute__(function_name)

if __name__ == "__main__":
    dlt1f = open("dltest1.py", 'r')
    dlt1text = dlt1f.read()
    dlt1f.close()
    dlt2f = open("dltest2.py", 'r')
    dlt2text = dlt2f.read()
    dlt2f.close()

    dlf = open("dltest.py", 'w')
    dlf.write(dlt1text)
    dlf.close()

    a = import_as_func("dltest")
    a()

    dlf = open("dltest.py", 'w')
    dlf.writelines(dlt2text)
    dlf.close()

    importlib.invalidate_caches()
    b = import_as_func("dltest")

    b()
    a()
