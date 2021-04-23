let rec f: 'a. (Belt.List.t<'a>, 'a) => int = (list, needle) =>
  switch list {
  | list{} => raise(Not_found)
  | list{head, ...rest} => head == needle ? 0 : 1 + f(rest, needle)
  }
