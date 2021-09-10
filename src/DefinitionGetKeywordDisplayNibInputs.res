let f = (definition: Definition.t, language: LanguageName.t): array<DisplayNib.t> =>
  DefinitionGetKeywordDisplayNibs.f(definition, language, true)
