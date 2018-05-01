import unittest
import librepcb as lp

class TestSymbol(unittest.TestCase):
    def setUp(self):
        wp = lp.determineWorkspacePath()
        symbol_uuid = "cb9e1ec9-e935-40e0-863e-7946730e8325" 
        self.symbol = lp.Symbol(wp.getPathTo("v0.1/libraries/local/LibrePCB_Base.lplib/sym/{}/".format(symbol_uuid)), True)

    def test_symbol_load(self):
        self.assertIsInstance(self.symbol, lp.Symbol)
        self.assertEqual(self.symbol.author, "LibrePCB")

    def test_pins(self):
        p = self.symbol.pins
        pin_index = 0
        pin_uuid = lp.Uuid("11d0b59a-11b0-4168-ac68-189c24c14f80")
        pin_name = "CRED"

        self.assertEqual(len(p), 4)
        with self.assertRaises(IndexError):
            p[4]

        self.assertEqual(p[pin_index].name, pin_name)
        self.assertEqual(p[pin_index].uuid, pin_uuid)
        #self.assertEqual(p[pin_uuid].name, pin_name)
        #self.assertEqual(p[pin_name].uuid, pin_uuid)

        # invalid keys
        #with self.assertRaises(RuntimeError):
        #    p["BLA"]
        #with self.assertRaises(RuntimeError):
        #    p[lp.Uuid("11d0b59a-11b0-4168-ac68-189c24c14f81")]

    def test_polygon(self):
        p = self.symbol.polygons

        p_index = 30

        self.assertEqual(len(p), 31)
        with self.assertRaises(IndexError):
            p[31]

        self.assertEqual(p[p_index].layerName, "sym_outlines")

    def test_ellipse(self):
        es = self.symbol.ellipses

        self.assertEqual(len(es), 1)
        with self.assertRaises(IndexError):
            es[1]

        e = es[0]
        self.assertEqual(e.layerName, "sym_outlines")
        self.assertEqual(e.lineWidth, lp.Length(254000))
        self.assertEqual(e.center, lp.Point(lp.Length(0), lp.Length(0)))
        self.assertEqual(e.rx, 179600)
        self.assertEqual(e.ry, 179600)
        self.assertEqual(e.rotation, 0)

    def test_text(self):
        ts = self.symbol.texts

        self.assertEqual(len(ts), 5)
        with self.assertRaises(IndexError):
            ts[5]

        t = ts[0]
        self.assertEqual(t.text, "#NAME")
        self.assertEqual(t.layerName, "sym_names")
        self.assertEqual(t.position, lp.Point(lp.Length(1000000), lp.Length(762000)))
        self.assertEqual(t.rotation, 0)
        self.assertEqual(t.height, 3175000)


if __name__ == "__main__":
    unittest.main()
