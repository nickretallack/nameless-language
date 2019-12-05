let f = (translatable: Translatable.t, language: LanguageName.t) =>
  Belt.Map.String.getExn(translatable.translations, language).text;
