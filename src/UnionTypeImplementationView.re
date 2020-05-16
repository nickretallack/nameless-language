[@react.component]
let make =
    (
      ~definitions: DefinitionMap.t,
      ~typeSet: TypeSet.t,
      ~languageName: LanguageName.t,
      ~emit,
    ) => {
  let (selectedType, setSelectedType) =
    React.useState(() => ValueType.AnyType);

  <div>
    <h1> {ReasonReact.string("Union")} </h1>
    <TypeSelectorView
      valueType=selectedType
      definitions
      languageName
      changeType={valueType => setSelectedType(_ => valueType)}
    />
    <button
      onClick={_event => emit(DefinitionAction.AddUnionType(selectedType))}>
      {ReasonReact.string("Add Type")}
    </button>
    <table>
      <tbody>
        {ReasonReact.array(
           Belt.Array.map(Belt.Set.toArray(typeSet), valueType =>
             <tr>
               <td>
                 <a
                   onClick={_event =>
                     emit(DefinitionAction.RemoveUnionType(valueType))
                   }>
                   {ReasonReact.string("x")}
                 </a>
               </td>
               <td>
                 {ReasonReact.string(
                    ValueTypeGetDisplayName.f(
                      valueType,
                      definitions,
                      languageName,
                    ),
                  )}
               </td>
             </tr>
           ),
         )}
      </tbody>
    </table>
  </div>;
};
