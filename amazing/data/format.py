

_, w, h, has = raw_input().split()
print _, w, h, has

w = int(w)
h = int(h)
L = ""
while True:
    try:
        L += raw_input()
    except:
        break

for i in range(h):
    print L[i*w:(i+1)*w]

