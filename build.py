import sys
import Syati.build

# Get the target region from the command line
if len(sys.argv) < 2:
    print("Error: Did not specify a target region.")
    sys.exit(1)

Syati.build.build(sys.argv[1], ["include", "Syati/include"])
