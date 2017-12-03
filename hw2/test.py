from random import randrange, choice, shuffle
from phe import paillier

# Pretend everyting is encrypted because -.-

# Z_n
N = 13

# Bit length
l = 3

# String formating for l bits 
bit_str = '{:0' + str(l) + 'b}'

# Generate pailliar keys
public_key, private_key = paillier.generate_paillier_keypair()

def secure_multiplication(a, b):
	### Party 1 ###
	ra = randrange(0, N)
	rb = randrange(0, N)

	a_prime = (a + public_key.encrypt(ra))
	b_prime = (b + public_key.encrypt(rb))

	### Party 2 ###
	h_a = private_key.decrypt(a_prime)
	h_b = private_key.decrypt(b_prime)

	h = (h_a * h_b) % N
	h_prime = public_key.encrypt(h)

	### Party 1 ###
	s = h_prime + (a * (N - rb))
	s_prime = s + (b * (N - ra))
	a_times_b = s_prime + (public_key.encrypt(ra * rb) * (N - 1))

	return a_times_b

def minimum(u, v):
	### Party 1 ###
	# Randomly choose functionality
	f = choice(["u > v", "u < v"])

	# Initalize H_0
	h_i = public_key.encrypt(0)

	# Random s and t
	s = []
	t = []

	# L list
	L = []

	for i in range(l):
		# Assume we can get each bit out of encrypted text
		u_i = public_key.encrypt(int(bit_str.format(x)[i]))
		v_i = public_key.encrypt(int(bit_str.format(y)[i]))

		# Secure minimum
		e_ui_times_vi = secure_multiplication(u_i, v_i)
		#print(private_key.decrypt(e_ui_times_vi) % N)

		# Assign w_i based on f
		w_i = 0
		if f == 'u > v':
			w_i = u_i + (e_ui_times_vi * (N - 1))
		else:
			w_i = v_i + (e_ui_times_vi * (N - 1))

		# G_i
		g_i = public_key.encrypt(int(bit_str.format(x)[i]) ^ int(bit_str.format(y)[i]))

		# H_i
		s.append(randrange(0, N))
		h_i = (h_i * s[-1]) + g_i

		# Phi
		phi_i = public_key.encrypt(-1) + h_i

		# L_i
		t.append(randrange(0, N))
		l_i = w_i + (phi_i * t[-1])
		L.append(l_i)

	# Permute L
	shuffle(L)

	### Party 2 ###
	M = [private_key.decrypt(x) % N for x in L]
	
	# Alpha
	alpha = 0
	if 1 in M:
		alpha = 1

	e_alpha = public_key.encrypt(alpha)

	### Party 1 ###
	if f == 'u > v':
		return e_alpha
	else:
		return (e_alpha + (public_key.encrypt(-1) * (N - 1))) # Double check


x = 7
y = 2

#c = secure_multiplication(a, b)
#print(private_key.decrypt(c) % N)

##### Phase 2 #####
### Party 1 ###
print("Party 1")
a = public_key.encrypt(x)
b = public_key.encrypt(y)

print("Perform Binary decomposition")

# Min Index
print("Perform Minimum")
min_index = public_key.encrypt(private_key.decrypt(minimum(a, b)) % N) # Mod Field
print("u < v: {}".format(private_key.decrypt(min_index)))

# Gamma
gamma = []
r = []
for i in range(l):
	r.append(randrange(0, N))
	gamma.append(public_key.encrypt(int(bit_str.format(y)[i]) - int(bit_str.format(x)[i])) + \
		public_key.encrypt(r[-1]))

# Permutation
#shuffle(gamma)

### Party 2 ### (permuted gamma and min_index)
d_min_index = private_key.decrypt(min_index)

m_prime = []
for i in range(l):
	m_prime.append(gamma[i] * d_min_index)

### Party 1 ### (m_prime)

# Lambda
lam = []
for i in range(l):
	lam.append(m_prime[i] + (min_index * (N - r[i])))


# Gamma
gamma = []
for i in range(l):
	gamma.append(public_key.encrypt(int(bit_str.format(x)[i])) + lam[i])

# Move back in field, and then to binary
for i in range(l):
	gamma[i] = public_key.encrypt((private_key.decrypt(gamma[i]) % N) % 2)

# Convert back into a single number (need to do because of phe limitations)
gamma = public_key.encrypt(int(''.join(str(x) for x in [private_key.decrypt(x) for x in gamma]), 2))


print("Min: {}".format(private_key.decrypt(gamma)))







"""
>>> from phe import paillier
>>> public_key, private_key = paillier.generate_paillier_keypair()
>>> secret_number_list = [3.141592653, 300, -4.6e-12]
>>> encrypted_number_list =se [public_key.encrypt(x) for x in secret_number_list]
>>> [private_key.decrypt(x) for x in encrypted_number_list]
[3.141592653, 300, -4.6e-12]
"""