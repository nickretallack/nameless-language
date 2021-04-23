let rec f: 'a. (list<'a>, 'a => bool) => int = (list, check) =>
  switch list {
  | list{} => raise(Not_found)
  | list{head, ...rest} => check(head) ? 0 : 1 + f(rest, check)
  }
