let f = (list, fn) =>
  ReasonReact.array(Array.of_list(List.mapi(fn, list)));
