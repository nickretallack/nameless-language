[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~valueType: ValueType.t,
      ~languageName: LanguageName.t,
      ~emit,
    ) => {
  let changeType = (valueType: ValueType.t): unit =>
    emit(DefinitionAction.ChangeLabeledType(valueType));

  <div>
    <h1> {ReasonReact.string("Label")} </h1>
    <table>
      <tbody>
        <tr>
          <td> {ReasonReact.string("Type: ")} </td>
          <td>
            <TypeSelectorView valueType definitions languageName changeType />
          </td>
        </tr>
      </tbody>
    </table>
  </div>;
};
