let f =
    (definition: Definition.t, language: LanguageName.t): list(DisplayNib.t) =>
  DefinitionGetKeywordDisplayNibs.f(definition, language, false);
