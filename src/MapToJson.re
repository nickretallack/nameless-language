let f = (map: Belt.Map.String.t('a), encode: 'a => Js.Json.t) =>
  Json.Encode.object_(
    Belt.List.map(Belt.Map.String.toList(map), ((key, item)) =>
      (key, encode(item))
    ),
  );
