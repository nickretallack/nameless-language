let f = (translatable: Translatable.t, language: LanguageName.t, text: string) => {
  ...translatable,
  translations:
    Belt.Map.String.update(
      translatable.translations,
      language,
      (maybeVettable: option(Vettable.t)) =>
      switch (maybeVettable) {
      | Some(vettable) => Some({...vettable, text})
      | None => Some({text, vetted: false, authorID: None})
      }
    ),
};
