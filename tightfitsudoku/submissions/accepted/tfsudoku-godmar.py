

B = [] 
for i in range(6):
    B.append(map(lambda x : x.replace("/", " ").split(), raw_input().split()))

inrow = [set(map(str, range(1, 10))) for _ in range(6)]
incol = [set(map(str, range(1, 10))) for _ in range(6)]
insqr = [set(map(str, range(1, 10))) for _ in range(6)]

def ri(r, c):
    return r/2*2+c/3

left = dict()

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

choices = {}
for r, row in enumerate(B):
    for c, cell in enumerate(row):    # 6 cells
        for i, v in enumerate(cell):
            if v == '-':
                choices[(r, c, i)] = inrow[r] & incol[c] & insqr[ri(r,c)]
                
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

    picks = []
    for (r, c, i), ch in choices.iteritems():
        if B[r][c][i] == '-':
            picks.append((len(ch), r, c, i, ch))

    picks.sort()

    l, r, c, i, left = picks[0]
    for k in left:
        cell = B[r][c]
        cell[i] = k

        # check x/y x < y condition
        if len(cell) == 2 and cell[1-i] != '-' and cell[0] > cell[1]:
            cell[i] = '-'
            continue

        undolog = []
        for (r0, c0, i0), ch in choices.iteritems():
            if B[r0][c0][i0] == '-' and (r0 == r or c0 == c or ri(r, c) == ri(r0, c0)):
                if k in choices[(r0, c0, i0)]:
                    undolog.append((r0, c0, i0))

        for u in undolog:
            choices[u].discard(k)

        if backtrack(dashes-1):
            return True

        cell[i] = '-'
        for u in undolog:
            choices[u].add(k)

    return False

backtrack(dashes)

