if symbol is None:
    raise RuntimeError("this script is supposed to be run while editing a symbol!")
print("modifying symbol with uuid {}".format(symbol.uuid))
for p in symbol.pins:
    p.name = p.name.lower()
    #p.rotation = p.rotation + lp.Angle(90000000)

for t in symbol.texts:
    t.text = t.text.lower()

for e in symbol.ellipses:
    e.rx = e.rx * 2

u = lp.Uuid.createRandom()
p = lp.SymbolPin(u, "DYNAMICAL"+str(len(symbol.pins)), lp.Point(lp.Length(2540000), lp.Length(2540000*len(symbol.pins))), lp.Length(2540000), lp.Angle(0))
symbol.pins.append(p)
