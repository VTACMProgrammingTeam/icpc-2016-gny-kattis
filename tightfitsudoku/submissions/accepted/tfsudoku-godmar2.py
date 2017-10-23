

B = [] 
for i in range(6):
    B.append(map(lambda x : x.replace("/", " ").split(), raw_input().split()))

inrow = [set(map(str, range(1, 10))) for _ in range(6)]
incol = [set(map(str, range(1, 10))) for _ in range(6)]
insqr = [set(map(str, range(1, 10))) for _ in range(6)]

def ri(r, c):
    return r/2*2+c/3

dashes = 0
for r, row in enumerate(B):
    for c, cell in enumerate(row):    # 6 cells
        for v in cell:
            if v != '-':
                inrow[r].remove(v)
                incol[c].remove(v)
                insqr[ri(r,c)].remove(v)
            else:
                dashes += 1
                
def outputboard(): # output board
    for row in B:
        for cell in row:
            if len(cell) == 2:
                print "%s/%s" % (cell[0], cell[1]),
            else:
                print cell[0],
        print

def backtrack(dashes):
    if dashes == 0:
        outputboard()
        return True

    #outputboard()
    #print
    choices = []
    for r, row in enumerate(B):
        for c, cell in enumerate(row):    # 6 cells
            for i, v in enumerate(cell):
                if v == '-':
                    left = inrow[r] & incol[c] & insqr[ri(r,c)]
                    choices.append((len(left), r, c, i, left))

    choices.sort()

    l, r, c, i, left = choices[0]
    for k in left:
        cell = B[r][c]
        cell[i] = k

        # check x/y x < y condition
        if len(cell) == 2 and cell[1-i] != '-' and cell[0] > cell[1]:
            cell[i] = '-'
            continue

        inrow[r].remove(k)
        incol[c].remove(k)
        insqr[ri(r,c)].remove(k)
        if backtrack(dashes-1):
            return True

        cell[i] = '-'
        inrow[r].add(k)
        incol[c].add(k)
        insqr[ri(r,c)].add(k)
     
    return False

backtrack(dashes)

