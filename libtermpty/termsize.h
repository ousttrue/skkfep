#pragma once

struct TermSize
{
  int Rows;
  int Cols;
};

TermSize
get_termsize();

TermSize
current_termsize();

