@react.component
let make = (
  ~definitionID: DefinitionID.t,
  ~definitions: DefinitionMap.t,
  ~implementation: GraphImplementation.t,
  ~definition: Definition.t,
  ~display: DefinitionDisplay.t,
  ~documentation: Documentation.t,
  ~languageName: LanguageName.t,
  ~emit: DefinitionAction.t => unit,
  ~error: AppError.t,
  ~stackFrame: option<MaterializedStackFrame.t>,
  ~urlHashRest: array<string>,
) => {
  let (state, dispatch) = ReactUpdate.useReducer(
    {
      open GraphState
      {
        pointers: Belt.Map.make(~id=module(PointerComparable.C)),
        error: None,
        selection: NoSelection,
        scroll: {x: 0.0, y: 0.0},
        zoom: 1.0,
      }
    },
    GraphReducer.f(emit, implementation),
  )
  <div id="graph-view">
    <nav id="graph-nav" className="nav-buttons">
      <a
        href={`#${definitionID}/documentation`}
        className={urlHashRest == ["documentation"] ? "active" : ""}>
        {React.string("Documentation")}
      </a>
      <a href={`#${definitionID}`} className={urlHashRest == [] ? "active" : ""}>
        {React.string("Implementation")}
      </a>
    </nav>
    {switch urlHashRest {
    | ["documentation"] =>
      <div id="documentation">
        <DefinitionHeaderView
          definitionID
          definitions
          documentation
          languageName
          emit
          error
          placeholder="(nameless function)"
        />
        {switch state.error {
        | Some(error) => <div className="error-message"> {React.string(error)} </div>
        | None => React.null
        }}
        {switch state.selection {
        | SelectedNib(explicitConnectionSide) => <>
            <button onClick={_event => emit(EvaluateNib(explicitConnectionSide))}>
              {React.string("Debug")}
            </button>
            <NodeMenuView
              emit definitions languageName nodes=implementation.nodes nib=explicitConnectionSide
            />
          </>
        | SelectedConnection(connectionSide) => <>
            <button
              onClick={_event =>
                emit(EvaluateNib({connectionSide: connectionSide, isSource: false}))}>
              {React.string("Debug")}
            </button>
            <button onClick={_event => emit(RemoveConnection(connectionSide))}>
              {React.string("Remove connection")}
            </button>
          </>
        | SelectedNodes(_) =>
          <button onClick={_event => dispatch(RemoveSelectedNodes)}>
            {React.string("Remove Node(s)")}
          </button>
        | NoSelection => React.null
        }}
        <h2> {React.string("Interface")} </h2>
        <InterfaceView
          definitions interface=implementation.interface documentation display emit languageName
        />
      </div>
    | _ => React.null
    }}
    <GraphImplementationView
      definitions implementation definition display languageName state stackFrame emit=dispatch
    />
  </div>
}
