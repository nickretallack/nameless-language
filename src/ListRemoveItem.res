let f = (list: list<'a>, element: 'a): list<'a> =>
  Belt.List.keep(list, listElement => element != listElement)
