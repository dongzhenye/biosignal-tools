# Loads mkmf which is used to make makefiles for Ruby extensions
require 'mkmf'

# Give it a name
extension_name = 'biosig'

# The destination
dir_config(extension_name, "..", ".." )

$LDFLAGS='-lbiosig -lz -lcholmod'

# Do the work
create_makefile(extension_name)
