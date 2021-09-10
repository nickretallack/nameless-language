@react.component
let make = (
  ~definitions: DefinitionMap.t,
  ~languageName: LanguageName.t,
  ~typedFields: RecordType.t,
  ~nibDocumentations: Belt.Map.String.t<Translatable.t>,
  ~isInput: bool,
  ~ordering: array<NibID.t>,
  ~emit,
) =>
  <table>
    <tbody>
      {React.array(
        Belt.Array.mapWithIndex(ordering, (index, nibID) =>
          <TypedFieldView
            key=nibID
            valueType={Belt.Map.String.getExn(typedFields, nibID)}
            name={Belt.Map.String.getExn(nibDocumentations, nibID)}
            index
            count={Belt.Array.length(ordering)}
            definitions
            languageName
            emit
            isInput
            nibID
          />
        ),
      )}
    </tbody>
  </table>
