# make sure librepcbsexpression.so is available in sys.path
import librepcbsexpression as sex

token = sex.createToken("bla")

string = sex.createString("blubb")
l = sex.createList("list")
l.appendChild(token, False)
l.appendChild(string, False)

l2 = sex.createList("sublist")
l.appendChild(l2, False)

serialized = l.toString(0)
print(serialized)

parsed = sex.parse(str(serialized), "test.lp")
print(parsed)
print(parsed.toString(0))

children = parsed.getChildren()
for i in range(len(children)):
	print("{}: {}".format(i, children[i].toString(0)))

testexpression = """
(librepcb_component
 (uuid 38fd7dc5-d607-4cff-9253-dbce6ccb4e18)
 (version "0.1")
 (author "hephaisto")
 (created 2018-01-12T05:24:30Z)
 (modified 2018-01-12T05:24:30Z)
 (deprecated false)
 (name "HT16K33")
 (description "")
 (keywords "")
 (category 1a83de82-d56c-4496-858f-d6a93d1a69ca)
 (schematic_only false)
 (default_value "")
 (prefix "")
 (signal 89efebc1-7f14-4902-b77f-9b2fb3d3eea5 (name "ROW4") (role passive)
  (required false) (negated false) (clock false) (forced_net "")
 )
)
"""

root = sex.parse(testexpression, "test.lp")
ch = root.getChildByPath("signal")
print(ch.toString(0))
print("name: {}".format(root.getName()))
