[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~wrappedType: option(ValueType.t),
      ~languageName: LanguageName.t,
      ~emit,
    ) => {
  let changeType = (valueType: ValueType.t): unit =>
    emit(DefinitionAction.ChangeLabeledType(Some(valueType)));

  <div>
    <h1> {ReasonReact.string("Label")} </h1>
    <label htmlFor="hasWrappedType">
      <input
        id="hasWrappedType"
        type_="checkbox"
        checked={Belt.Option.isSome(wrappedType)}
        onChange={_ =>
          emit(
            DefinitionAction.ChangeLabeledType(
              switch (wrappedType) {
              | None => Some(ValueType.AnyType)
              | Some(_) => None
              },
            ),
          )
        }
      />
      {ReasonReact.string("Contains a value")}
    </label>
    {switch (wrappedType) {
     | Some(valueType) =>
       <table>
         <tbody>
           <tr>
             <td> {ReasonReact.string("Type: ")} </td>
             <td>
               <TypeSelectorView
                 valueType
                 definitions
                 languageName
                 changeType
               />
             </td>
           </tr>
         </tbody>
       </table>
     | None => ReasonReact.null
     }}
  </div>;
};
