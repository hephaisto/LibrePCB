import unittest
import librepcb as lp
from common import getWorkspaceLocation

class TestPackage(unittest.TestCase):
    def setUp(self):
        wp = getWorkspaceLocation()
        package_uuid = "60b715fa-bd1d-4d96-b153-2a28208f32fb" 
        self.package = lp.Package(wp.getPathTo("v0.1/libraries/local/LibrePCB_Base.lplib/pkg/{}/".format(package_uuid)), True)

    def test_symbol_load(self):
        self.assertIsInstance(self.package, lp.Package)
        self.assertEqual(self.package.author, "LibrePCB")

    def test_pads(self):
        p = self.package.pads
        pad_index = 0
        pad_uuid = lp.Uuid("4f6ec20b-c2e7-47d9-ba0e-8cde302966c5")
        pad_name = "1"

        self.assertEqual(len(p), 2)
        with self.assertRaises(IndexError):
            p[2]

        self.assertEqual(p[pad_index].name, pad_name)
        self.assertEqual(p[pad_index].uuid, pad_uuid)
        self.assertEqual(p[pad_uuid].name, pad_name)
        self.assertEqual(p[pad_name].uuid, pad_uuid)

        # invalid keys
        with self.assertRaises(RuntimeError):
            p["BLA"]
        with self.assertRaises(RuntimeError):
            p[lp.Uuid("11d0b59a-11b0-4168-ac68-189c24c14f81")]

    def test_pad_list(self):
        p = self.package.pads
        len_start = len(p)
        pad = p[len_start-1]
        p.remove(pad)
        len_removed = len(p)
        self.assertEqual(len_removed, len_start-1)
        p.append(pad)
        self.assertEqual(len(p), len_start)

    def test_footprint(self):
        f = self.package.footprints[0]
        self.assertEqual(f.uuid, lp.Uuid("328e4ae0-a0f1-4d92-87b5-1695afbe98e1"))
        p = f.pads
        
        self.assertEqual(len(p), 2)
        self.assertEqual(len(f.polygons), 6)
        self.assertEqual(len(f.texts), 2)
        self.assertEqual(len(f.ellipses), 0)
        self.assertEqual(len(f.holes), 0)
        self.assertEqual(p[lp.Uuid("4f6ec20b-c2e7-47d9-ba0e-8cde302966c5")].rotation, lp.Angle(0))

if __name__ == "__main__":
    unittest.main()
