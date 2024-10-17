from heu import phe

kit = phe.setup(phe.SchemaType.ZPaillier, 2048)
encryptor = kit.encryptor()
evaluator = kit.evaluator()
decryptor = kit.decryptor()
def encryptByZPaillier(value):
     return encryptor.encrypt_raw(value)

def addInPlaceReturnCiperValue(c1,c2):
    evaluator.add_inplace(c1, c2)
    return c1

def addInPlaceReturnPlainValue(c1,c2):
    evaluator.add_inplace(c1, c2)
    return decryptor.decrypt_raw(c1)


