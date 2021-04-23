let f: 'a. (Belt.Map.String.t<'a>, Belt.Map.String.t<'a>) => Belt.Map.String.t<'a> = (map1, map2) =>
  Belt.Map.String.reduce(map1, map2, (acc, key, value) => Belt.Map.String.set(acc, key, value))
