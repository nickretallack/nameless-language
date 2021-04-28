@react.component
let make = (~languageName: LanguageName.t, ~emit) => {
  <div>
    {React.array(
      Belt.Array.mapWithIndex(DefinitionMakers.v, (index, (name, maker)) =>
        <a
          className="maker"
          key={string_of_int(index)}
          onClick={_event => {
            let newDefinitionID = RandomIDMake.f()
            emit(AppAction.CreateDefinition(maker(languageName)))
            RescriptReactRouter.push("#" ++ newDefinitionID)
          }}>
          {React.string(name)}
        </a>
      ),
    )}
  </div>
}
