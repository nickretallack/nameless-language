let f = (fn, map) =>
  ReasonReact.array(Array.map(fn, Belt.Map.toArray(map)));
