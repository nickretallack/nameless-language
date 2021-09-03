@react.component
let make = (
  ~ordering: list<NibID.t>,
  ~sourceValues: Belt.Map.t<ConnectionSide.t, Value.t, ConnectionSideComparable.C.identity>,
  ~nibDocumentations: Belt.Map.String.t<Translatable.t>,
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~execution: Execution.t,
  ~nibToSource: NibID.t => ConnectionSide.t,
) =>
  <div>
    {React.array(
      Belt.List.toArray(
        Belt.List.mapWithIndex(ordering, (_index, nibID) =>
          switch Belt.Map.get(sourceValues, nibToSource(nibID)) {
          | None => React.null
          | Some(value) =>
            let name = TranslatableGetText.f(
              Belt.Map.String.getExn(nibDocumentations, nibID),
              languageName,
            )
            let value = ValueDisplay.f(value, execution, definitions, languageName)
            <div key={nibID}> {React.string(`${name}: ${value}`)} </div>
          }
        ),
      ),
    )}
  </div>
