let rec f: 'a. (Belt.List.t('a), 'a) => int =
  (list, needle) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => head == needle ? 0 : 1 + f(rest, needle)
    };
