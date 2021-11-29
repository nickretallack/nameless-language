let f = (translatable: Translatable.t, query: string, languageName: LanguageName.t): bool => {
  switch Belt.Map.String.get(translatable.translations, languageName) {
  | Some(vettable) =>
    Js.String.includes(
      Js.String.toLocaleLowerCase(query),
      Js.String.toLocaleLowerCase(vettable.text),
    )
  | None => false
  }
}
