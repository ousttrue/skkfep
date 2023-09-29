#include "statusline.h"

StatusType g_status = {};

namespace status {

StatusType
type()
{
  return g_status;
}

void
type(StatusType s)
{
  g_status = s;
}

}
