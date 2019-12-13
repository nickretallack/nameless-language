let f =
    (map: Belt.Map.String.t('a), key: string, updater: 'a => 'a)
    : Belt.Map.String.t('a) =>
  Belt.Map.String.update(map, key, (maybeValue: option('a)) =>
    switch (maybeValue) {
    | Some(value) => Some(updater(value))
    | None => raise(Not_found)
    }
  );
