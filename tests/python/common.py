import os
import librepcb as lp

for k, v in os.environ.items():
    print("{}={}".format(k, v))

def getWorkspaceLocation():
    try:
        wp = os.environ["TEST_WORKSPACE_LOCATION"]
        print("getting workspace from TEST_WORKSPACE_LOCATION: \"{}\"".format(wp))
        return lp.FilePath(wp)
    except KeyError:
        print("TEST_WORKSPACE_LOCATION not defined")
        wp = lp.determineWorkspacePath()
        if len(wp.toStr()) > 0:
            return wp
        else:
            default_wp = "./dev/demo-workspace"
            print("default workspace empty, defaulting to \"{}\"".format(default_wp))
            return lp.FilePath(wp)

print("using workspace location: \"{}\"".format(getWorkspaceLocation().toStr()))
