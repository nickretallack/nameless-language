let f = (text: string, language: LanguageName.t): Translatable.t => {
  sourceLanguage: language,
  translations:
    Belt.Map.String.fromArray([|
      (language, Vettable.{text, vetted: true, authorID: None}),
    |]),
};
