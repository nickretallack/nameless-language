let f = (nibs: array<(NibID.t, string)>) =>
  Belt.Map.String.fromArray(
    Belt.Array.map(nibs, ((nibID, text)) => (nibID, TranslatableMake.f(text, "en"))),
  )
