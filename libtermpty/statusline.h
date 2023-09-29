#pragma once

enum class StatusType
{
  NoStatusLine = 0,
  HaveStatusLine,
  UseBottomLine
};

namespace status {

StatusType
type();

void
type(StatusType s);

}
