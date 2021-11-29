let f = (documentation: Documentation.t, query: string, languageName: LanguageName.t): bool =>
  TranslatableMatches.f(documentation.name, query, languageName)
