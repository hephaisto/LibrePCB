print("modifying symbol with uuid {}".format(symbol.uuid))
for p in symbol.pins:
    p.name = p.name.lower()
