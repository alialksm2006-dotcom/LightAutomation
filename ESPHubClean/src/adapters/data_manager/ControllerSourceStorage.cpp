#include "ControllerSourceStorage.h"
 int ControllerSourceStorage::id = 0;
 std::vector<std::unique_ptr<ControllerSource>> ControllerSourceStorage::sources;