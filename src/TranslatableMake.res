let f = (text: string, language: LanguageName.t): Translatable.t => {
  sourceLanguage: language,
  translations: Belt.Map.String.fromArray([
    (
      language,
      {
        open Vettable
        {text: text, vetted: true, authorID: None}
      },
    ),
  ]),
}
