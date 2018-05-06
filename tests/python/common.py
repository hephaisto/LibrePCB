import os
import librepcb as lp

def getWorkspaceLocation():
    try:
        wp = os.environ["TEST_WORKSPACE_LOCATION"]
        return lp.FilePath(wp)
    except KeyError:
        return lp.determineWorkspacePath()

print("using workspace location: \"{}\"".format(str(getWorkspaceLocation())))
