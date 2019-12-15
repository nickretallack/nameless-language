type t = {
  execution: option(Execution.t),
  definitions: DefinitionMap.t,
  error: AppError.t,
  languageName: LanguageName.t,
};
