let rec f: 'a. (list<'a>, 'a) => (list<'a>, list<'a>) = (list, needle) => {
  switch list {
  | list{} => raise(Not_found)
  | list{head, ...rest} =>
    if head == needle {
      (list{head}, rest)
    } else {
      let (left, right) = f(rest, needle)
      (list{head, ...left}, right)
    }
  }
}
