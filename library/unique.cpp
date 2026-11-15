// @no-include（main の中に貼るスニペット。#include では使えない）
sort(all($1));
$1.erase(unique($1.begin(), $1.end()), $1.end());
