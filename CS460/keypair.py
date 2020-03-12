# import Crypto
# from Crypto.PublicKey import RSA
# from Crypto.Random import get_random_bytes
# from Crypto.Cipher import AES
# from Crypto.Cipher import PKCS1_OAEP
#
# aes_key = get_random_bytes(16)
# cipher = AES.new(aes_key, AES.MODE_EAX)
#
# print(aes_key);
#
# key = RSA.generate(2048)
# private_key = key.export_key()
# file_out = open("private.pem", "wb")
# file_out.write(private_key)
#
# public_key = key.publickey().export_key()
# file_out = open("receiver.pem", "wb")
# file_out.write(public_key)
#
# useKey = public_key.publickey()
# encrypted = useKey.encrypt('encrypt this message', 32)
# print(encrypted)
# from Crypto.Cipher import AES
# from Crypto.Random import get_random_bytes
# from Crypto.PublicKey import RSA
#
# # generate a random AES key to encrypt
# rand = get_random_bytes(16)
# aes_key = AES.new(rand, AES.MODE_EAX)
# f = open("aes.pem", "wb")
# f.write(aes_key.export_key())
# f.close()
#
# # generate a RSA key
# key = RSA.generate(2048)
# private_key = key.export_key()
# f = open("private.pem", "wb")
# f.write(private_key)
# f.close()
#
# public_key = key.publickey().export_key()
# f = open("receiver.pem", "wb")
# f.write(public_key)
# f.close()



# from Crypto.Random import get_random_bytes
from Crypto.Cipher import PKCS1_OAEP
from Crypto.PublicKey import RSA
from Crypto.Cipher import AES
from Crypto import Random

# aeskey = Random.new().read(32)
# iv = Random.new().read(AES.block_size)
# cipher = AES.new(aeskey, AES.MODE_CFB, iv)


aeskey =  Random.new().read(32)
cipher = AES.new(aeskey, AES.MODE_EAX)
msg = cipher.encrypt(b'Attack at dawn')

message = aeskey
random_generator = Random.new().read
rsakey = RSA.generate(1024, random_generator)
cipher = PKCS1_OAEP.new(rsakey.publickey())
ciphertext = cipher.encrypt(message)

private_key = RSA.import_key(cipher)
cipher_rsa = PKCS1_OAEP.new(private_key)
aeskey2 = cipher_rsa.decrypt(ciphertext)

cipher2 = AES.new(aeskey2, AES.MODE_EAX)
data = cipher2.decrypt(msg)

print(data)
