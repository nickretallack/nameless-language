// This code and the functions that generate it should eventually be removed when we begin serving the definitions from the repository.

let example = GraphMake.f(
  "en",
  ~name="Example Definition",
  ~description="An example function",
  ~inputs=[("in1", "In 1"), ("in2", "In 2")],
  ~outputs=[("out1", "Out 1"), ("out2", "Out 2")],
  ~nodes=[
    (
      "node1",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
        scope: GraphScope,
      },
    ),
    (
      "node2",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
        scope: GraphScope,
      },
    ),
    (
      "node3",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "example"}),
        scope: GraphScope,
      },
    ),
  ],
  ~connections=[
    (
      {node: NodeConnection("node1"), nib: NibConnection("in1")},
      {node: NodeConnection("node2"), nib: NibConnection("out1")},
    ),
    (
      {node: NodeConnection("node1"), nib: NibConnection("in2")},
      {node: NodeConnection("node2"), nib: NibConnection("out2")},
    ),
    (
      {node: GraphConnection, nib: NibConnection("out2")},
      {node: NodeConnection("node1"), nib: NibConnection("out2")},
    ),
    (
      {node: NodeConnection("node2"), nib: NibConnection("in1")},
      {node: GraphConnection, nib: NibConnection("in1")},
    ),
  ],
  (),
)

let simple = GraphMake.f(
  "en",
  ~name="Simple Definition",
  ~description="A simple function",
  ~inputs=[("in1", "In 1"), ("in2", "In 2")],
  ~outputs=[("out1", "Out 1"), ("out2", "Out 2")],
  ~nodes=[
    (
      "node1",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "node2",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "node3",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
        scope: GraphScope,
      },
    ),
  ],
  ~connections=[
    (
      {node: GraphConnection, nib: NibConnection("out1")},
      {node: NodeConnection("node3"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("node3"), nib: NibConnection("left")},
      {node: NodeConnection("node1"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("node3"), nib: NibConnection("right")},
      {node: NodeConnection("node2"), nib: ValueConnection},
    ),
  ],
  (),
)

let keydown = DefinitionMake.f(
  "en",
  ~implementation=ConstantImplementation(TextValue("keydown")),
  (),
)

let one = DefinitionMake.f("en", ~implementation=ConstantImplementation(NumberValue(1.0)), ())

let point = DefinitionMake.f(
  "en",
  ~name="2D Point",
  ~description="X and Y coordinates",
  ~inputs=[("x", "X"), ("y", "Y")],
  ~implementation=RecordTypeImplementation(
    Belt.Map.String.fromArray([
      ("x", ValueType.PrimitiveValueType(NumberType)),
      ("y", ValueType.PrimitiveValueType(NumberType)),
    ]),
  ),
  (),
)

let makeReference = DefinitionMake.f(
  "en",
  ~name="Make Reference",
  ~description="Create a shared reference to the supplied value.  Anyone you pass it to can read it and write to it.",
  ~inputs=[("value", "Value")],
  ~outputs=[("reference", "Reference")],
  ~implementation=ExternalImplementation({
    name: "makeReference",
    interface: {
      input: Belt.Map.String.fromArray([("value", ValueType.AnyType)]),
      output: Belt.Map.String.fromArray([
        ("reference", ValueType.ReferenceType(ValueType.AnyType)),
      ]),
    },
  }),
  (),
)

let setReference = DefinitionMake.f(
  "en",
  ~name="Set Reference Value",
  ~description="Update a reference to contain a different value.  Other code with the reference will be able to read this new value.",
  ~inputs=[("reference", "Reference"), ("newValue", "New Value"), (";", ";")],
  ~outputs=[(";", ";")],
  ~implementation=ExternalImplementation({
    name: "setReference",
    interface: {
      input: Belt.Map.String.fromArray([
        ("reference", ValueType.ReferenceType(ValueType.AnyType)),
        ("newValue", ValueType.AnyType),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([(";", ValueType.SequencerType)]),
    },
  }),
  (),
)

let getReference = DefinitionMake.f(
  "en",
  ~name="Get Reference Value",
  ~description="Get the value a reference points to.",
  ~inputs=[("reference", "Reference"), (";", ";")],
  ~outputs=[("value", "Value"), (";", ";")],
  ~implementation=ExternalImplementation({
    name: "getReference",
    interface: {
      input: Belt.Map.String.fromArray([
        ("reference", ValueType.ReferenceType(ValueType.AnyType)),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([
        ("value", ValueType.AnyType),
        (";", ValueType.SequencerType),
      ]),
    },
  }),
  (),
)

let concatenateText = DefinitionMake.f(
  "en",
  ~name="Concatenate Text",
  ~description="Combine two strings into one.",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "concatenateText",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(TextType)),
        ("right", ValueType.PrimitiveValueType(TextType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.PrimitiveValueType(TextType))]),
    },
  }),
  (),
)

let plus = DefinitionMake.f(
  "en",
  ~name="+",
  ~description="Add two numbers, resulting in a number.",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "+",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(NumberType)),
        ("right", ValueType.PrimitiveValueType(NumberType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.PrimitiveValueType(NumberType))]),
    },
  }),
  (),
)

let minus = DefinitionMake.f(
  "en",
  ~name="-",
  ~description="Subtract two numbers, resulting in a number.",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "-",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(NumberType)),
        ("right", ValueType.PrimitiveValueType(NumberType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.PrimitiveValueType(NumberType))]),
    },
  }),
  (),
)

let times = DefinitionMake.f(
  "en",
  ~name="*",
  ~description="Multiply two numbers, resulting in a number.",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "*",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(NumberType)),
        ("right", ValueType.PrimitiveValueType(NumberType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.PrimitiveValueType(NumberType))]),
    },
  }),
  (),
)

let divide = DefinitionMake.f(
  "en",
  ~name="/",
  ~description="Subtract two numbers, resulting in a number.",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "/",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(NumberType)),
        ("right", ValueType.PrimitiveValueType(NumberType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.PrimitiveValueType(NumberType))]),
    },
  }),
  (),
)

let pointExample = GraphMake.f(
  "en",
  ~name="Point Example",
  ~description="",
  ~inputs=[],
  ~outputs=[("x", "X"), ("y", "Y")],
  ~nodes=[
    (
      "constructor",
      {
        kind: DefinedNode({kind: ConstructorNode, definitionID: "point"}),
        scope: GraphScope,
      },
    ),
    (
      "destructure",
      {
        kind: DefinedNode({kind: AccessorNode, definitionID: "point"}),
        scope: GraphScope,
      },
    ),
    (
      "x",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "y",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
  ],
  ~connections=[
    (
      {node: NodeConnection("constructor"), nib: NibConnection("x")},
      {node: NodeConnection("x"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("constructor"), nib: NibConnection("y")},
      {node: NodeConnection("y"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("destructure"), nib: ValueConnection},
      {node: NodeConnection("constructor"), nib: ValueConnection},
    ),
    (
      {node: GraphConnection, nib: NibConnection("x")},
      {node: NodeConnection("destructure"), nib: NibConnection("x")},
    ),
    (
      {node: GraphConnection, nib: NibConnection("y")},
      {node: NodeConnection("destructure"), nib: NibConnection("y")},
    ),
  ],
  (),
)

let exampleInterface = DefinitionMake.f(
  "en",
  ~name="Example Interface",
  ~inputs=[("in-left", "Left"), ("in-right", "Right")],
  ~outputs=[("out-left", "Left"), ("out-right", "Right")],
  ~implementation=InterfaceImplementation({
    input: Belt.Map.String.fromArray([
      ("in-left", ValueType.PrimitiveValueType(NumberType)),
      ("in-right", ValueType.PrimitiveValueType(NumberType)),
    ]),
    output: Belt.Map.String.fromArray([
      ("out-left", ValueType.PrimitiveValueType(NumberType)),
      ("out-right", ValueType.PrimitiveValueType(NumberType)),
    ]),
  }),
  (),
)

let interfaceExample = GraphMake.f(
  "en",
  ~name="Interface Example",
  ~outputs=[("result", "Result")],
  ~nodes=[
    (
      "definition",
      {
        scope: GraphScope,
        kind: DefinedNode({
          kind: FunctionDefinitionNode,
          definitionID: "example-interface",
        }),
      },
    ),
    (
      "plus1",
      {
        scope: GraphScope,
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
      },
    ) /*
         (
           "plus2",
           {
             scope: NodeScope("definition"),
             kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
           },
         ),
         (
           "plus3",
           {
             scope: NodeScope("definition"),
             kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
           },
         ),
         (
           "plus4",
           {
             scope: GraphScope,
             kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
           },
         ), */,
  ],
  ~connections=[/* (
           {node: GraphConnection, nib: NibConnection("result")},
           {node: NodeConnection("definition"), nib: ValueConnection},
         ),
         (
           {node: NodeConnection("definition"), nib: NibConnection("out-left")},
           {node: NodeConnection("plus1"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus1"), nib: NibConnection("left")},
           {node: NodeConnection("plus3"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus2"), nib: NibConnection("left")},
           {node: NodeConnection("plus3"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus1"), nib: NibConnection("right")},
           {node: NodeConnection("plus2"), nib: NibConnection("result")},
         ),
         (
           {
             node: NodeConnection("definition"),
             nib: NibConnection("out-right"),
           },
           {node: NodeConnection("plus4"), nib: NibConnection("result")},
         ),
         (
           {node: NodeConnection("plus3"), nib: NibConnection("right")},
           {
             node: NodeConnection("definition"),
             nib: NibConnection("in-right"),
           },
         ), */],
  (),
)

let nestedInlineExample = GraphMake.f(
  "en",
  ~name="Nested Inline Functions Example",
  ~outputs=[("result", "Result")],
  ~nodes=[
    (
      "definition",
      {
        scope: GraphScope,
        kind: DefinedNode({
          kind: FunctionDefinitionNode,
          definitionID: "example-interface",
        }),
      },
    ),
    (
      "definition2",
      {
        scope: NodeScope("definition"),
        kind: DefinedNode({
          kind: FunctionDefinitionNode,
          definitionID: "example-interface",
        }),
      },
    ),
    (
      "definition3",
      {
        scope: GraphScope,
        kind: DefinedNode({
          kind: FunctionDefinitionNode,
          definitionID: "example-interface",
        }),
      },
    ),
    (
      "plus",
      {
        scope: NodeScope("definition3"),
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
      },
    ),
    (
      "plus2",
      {
        scope: GraphScope,
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "plus"}),
      },
    ),
  ],
  ~connections=[
    (
      {node: NodeConnection("plus"), nib: NibConnection("right")},
      {node: NodeConnection("plus2"), nib: NibConnection("result")},
    ),
  ],
  (),
)

let yesLabel = DefinitionMake.f("en", ~name="Yes", ~implementation=SymbolImplementation, ())

let noLabel = DefinitionMake.f("en", ~name="No", ~implementation=SymbolImplementation, ())

let booleanUnion = DefinitionMake.f(
  "en",
  ~name="Boolean",
  ~implementation=UnionTypeImplementation(
    TypeSet.fromArray([ValueType.DefinedValueType("yes"), ValueType.DefinedValueType("no")]),
  ),
  (),
)

let somethingLabel = DefinitionMake.f(
  "en",
  ~name="Something",
  ~implementation=LabeledTypeImplementation(AnyType),
  (),
)

let branch = DefinitionMake.f(
  "en",
  ~name="Branch",
  ~description="Conditionally evaluate.",
  ~inputs=[("if", "If"), ("then", "Then"), ("else", "Otherwise")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "branch",
    interface: {
      input: Belt.Map.String.fromArray([
        ("if", ValueType.DefinedValueType("boolean")),
        ("then", ValueType.AnyType),
        ("else", ValueType.AnyType),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.AnyType)]),
    },
  }),
  (),
)

let equals = DefinitionMake.f(
  "en",
  ~name="=",
  ~inputs=[("left", ""), ("right", "")],
  ~outputs=[("result", "")],
  ~implementation=ExternalImplementation({
    name: "=",
    interface: {
      input: Belt.Map.String.fromArray([("left", ValueType.AnyType), ("right", ValueType.AnyType)]),
      output: Belt.Map.String.fromArray([("result", ValueType.DefinedValueType("boolean"))]),
    },
  }),
  (),
)

let lessThan = DefinitionMake.f(
  "en",
  ~name="<",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: "<",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(NumberType)),
        ("right", ValueType.PrimitiveValueType(NumberType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.DefinedValueType("boolean"))]),
    },
  }),
  (),
)

let greaterThan = DefinitionMake.f(
  "en",
  ~name=">",
  ~inputs=[("left", "Left"), ("right", "Right")],
  ~outputs=[("result", "Result")],
  ~implementation=ExternalImplementation({
    name: ">",
    interface: {
      input: Belt.Map.String.fromArray([
        ("left", ValueType.PrimitiveValueType(NumberType)),
        ("right", ValueType.PrimitiveValueType(NumberType)),
      ]),
      output: Belt.Map.String.fromArray([("result", ValueType.DefinedValueType("boolean"))]),
    },
  }),
  (),
)

let keyboardEventListener = DefinitionMake.f(
  "en",
  ~name="Keyboard Event Handler",
  ~inputs=[("code", "code")],
  ~outputs=[(";", ";")],
  ~implementation=InterfaceImplementation({
    input: Belt.Map.String.fromArray([("code", ValueType.PrimitiveValueType(TextType))]),
    output: Belt.Map.String.fromArray([(";", ValueType.SequencerType)]),
  }),
  (),
)

let addKeyboardEventListener = DefinitionMake.f(
  "en",
  ~name="Add Keyboard Event Handler",
  ~inputs=[("handler", "Handler"), (";", ";")],
  ~outputs=[(";", ";")],
  ~implementation=ExternalImplementation({
    name: "addKeyboardEventListener",
    interface: {
      input: Belt.Map.String.fromArray([
        ("handler", ValueType.DefinedValueType("keyboardEventListener")),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([(";", ValueType.SequencerType)]),
    },
  }),
  (),
)

let scheduledEventHandler = DefinitionMake.f(
  "en",
  ~name="Scheduled Event Handler",
  ~inputs=[],
  ~outputs=[(";", ";")],
  ~implementation=InterfaceImplementation({
    input: Belt.Map.String.fromArray([]),
    output: Belt.Map.String.fromArray([(";", ValueType.SequencerType)]),
  }),
  (),
)

let scheduleRecurringEvent = DefinitionMake.f(
  "en",
  ~name="Schedule Recurring Event",
  ~inputs=[("handler", "Handler"), ("interval", "interval"), (";", ";")],
  ~outputs=[("identifier", "Identifier"), (";", ";")],
  ~implementation=ExternalImplementation({
    name: "scheduleRecurringEvent",
    interface: {
      input: Belt.Map.String.fromArray([
        ("handler", ValueType.DefinedValueType("scheduledEventHandler")),
        ("interval", ValueType.PrimitiveValueType(IntegerType)),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([
        (";", ValueType.SequencerType),
        ("identifier", ValueType.ScheduledEventIdentifierType),
      ]),
    },
  }),
  (),
)

let scheduleAnimationFrame = DefinitionMake.f(
  "en",
  ~name="Schedule Animation Frame",
  ~inputs=[("handler", "Handler"), (";", ";")],
  ~outputs=[("identifier", "Identifier"), (";", ";")],
  ~implementation=ExternalImplementation({
    name: "scheduleAnimationFrame",
    interface: {
      input: Belt.Map.String.fromArray([
        ("handler", ValueType.DefinedValueType("scheduledEventHandler")),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([
        (";", ValueType.SequencerType),
        ("identifier", ValueType.ScheduledEventIdentifierType),
      ]),
    },
  }),
  (),
)

let prerequisite = DefinitionMake.f(
  "en",
  ~name="Prerequisite",
  ~inputs=[("value", "Value"), (";", ";")],
  ~outputs=[("value", "Value"), (";", ";")],
  ~implementation=ExternalImplementation({
    name: "prerequisite",
    interface: {
      input: Belt.Map.String.fromArray([
        ("value", ValueType.AnyType),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([
        ("value", ValueType.AnyType),
        (";", ValueType.SequencerType),
      ]),
    },
  }),
  (),
)

let htmlCreateElement = DefinitionMake.f(
  "en",
  ~name="Create HTML Element",
  ~inputs=[("tagName", "Tag Name")],
  ~outputs=[("element", "Element")],
  ~implementation=ExternalImplementation({
    name: "htmlCreateElement",
    interface: {
      input: Belt.Map.String.fromArray([
        ("tagName", ValueType.PrimitiveValueType(PrimitiveValueType.TextType)),
      ]),
      output: Belt.Map.String.fromArray([("element", ValueType.HTMLElementType)]), // TODO
    },
  }),
  (),
)

let htmlSetCssProperty = DefinitionMake.f(
  "en",
  ~name="Set CSS Property",
  ~inputs=[("element", "Element"), ("property", "Property"), ("value", "Value"), (";", ";")],
  ~outputs=[(";", ";")],
  ~implementation=ExternalImplementation({
    name: "htmlSetCssProperty",
    interface: {
      input: Belt.Map.String.fromArray([
        ("element", ValueType.HTMLElementType),
        ("property", ValueType.PrimitiveValueType(PrimitiveValueType.TextType)),
        ("value", ValueType.PrimitiveValueType(PrimitiveValueType.TextType)),
      ]),
      output: Belt.Map.String.fromArray([(";", ValueType.SequencerType)]),
    },
  }),
  (),
)

let log = DefinitionMake.f(
  "en",
  ~name="Log",
  ~inputs=[("value", "Value"), (";", ";")],
  ~outputs=[(";", ";")],
  ~implementation=ExternalImplementation({
    name: "log",
    interface: {
      input: Belt.Map.String.fromArray([
        ("value", ValueType.AnyType),
        (";", ValueType.SequencerType),
      ]),
      output: Belt.Map.String.fromArray([(";", ValueType.SequencerType)]),
    },
  }),
  (),
)

let somethingExample = GraphMake.f(
  "en",
  ~name="Something example",
  ~outputs=[("output", "Output")],
  ~nodes=[
    (
      "something",
      {
        kind: DefinedNode({kind: ConstructorNode, definitionID: "something"}),
        scope: GraphScope,
      },
    ),
  ],
  (),
)

let factorial = GraphMake.f(
  "en",
  ~name="Factorial",
  ~outputs=[("result", "Result")],
  ~inputs=[("input", "Input")],
  ~nodes=[
    (
      "one",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "branch",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "branch"}),
        scope: GraphScope,
      },
    ),
    (
      "less-than",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "less-than"}),
        scope: GraphScope,
      },
    ),
    (
      "times",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "times"}),
        scope: GraphScope,
      },
    ),
    (
      "minus",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "minus"}),
        scope: GraphScope,
      },
    ),
    (
      "factorial",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "factorial"}),
        scope: GraphScope,
      },
    ),
  ],
  ~connections=[
    (
      {node: GraphConnection, nib: NibConnection("result")},
      {node: NodeConnection("branch"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("branch"), nib: NibConnection("if")},
      {node: NodeConnection("less-than"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("branch"), nib: NibConnection("then")},
      {node: NodeConnection("one"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("less-than"), nib: NibConnection("left")},
      {node: GraphConnection, nib: NibConnection("input")},
    ),
    (
      {node: NodeConnection("less-than"), nib: NibConnection("right")},
      {node: NodeConnection("one"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("branch"), nib: NibConnection("else")},
      {node: NodeConnection("times"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("times"), nib: NibConnection("left")},
      {node: GraphConnection, nib: NibConnection("input")},
    ),
    (
      {node: NodeConnection("times"), nib: NibConnection("right")},
      {node: NodeConnection("factorial"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("factorial"), nib: NibConnection("input")},
      {node: NodeConnection("minus"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("minus"), nib: NibConnection("left")},
      {node: GraphConnection, nib: NibConnection("input")},
    ),
    (
      {node: NodeConnection("minus"), nib: NibConnection("right")},
      {node: NodeConnection("one"), nib: ValueConnection},
    ),
  ],
  (),
)

let factorial2 = GraphMake.f(
  "en",
  ~name="Factorial",
  ~outputs=[("result", "Result")],
  ~inputs=[("input", "Input")],
  ~nodes=[
    (
      "one1",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "one2",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "one3",
      {
        kind: DefinedNode({kind: ValueNode, definitionID: "one"}),
        scope: GraphScope,
      },
    ),
    (
      "branch",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "branch"}),
        scope: GraphScope,
      },
    ),
    (
      "less-than",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "less-than"}),
        scope: GraphScope,
      },
    ),
    (
      "times",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "times"}),
        scope: GraphScope,
      },
    ),
    (
      "minus",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "minus"}),
        scope: GraphScope,
      },
    ),
    (
      "factorial",
      {
        kind: DefinedNode({kind: FunctionCallNode, definitionID: "factorial"}),
        scope: GraphScope,
      },
    ),
  ],
  ~connections=[
    (
      {node: GraphConnection, nib: NibConnection("result")},
      {node: NodeConnection("branch"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("branch"), nib: NibConnection("if")},
      {node: NodeConnection("less-than"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("branch"), nib: NibConnection("then")},
      {node: NodeConnection("one1"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("less-than"), nib: NibConnection("left")},
      {node: GraphConnection, nib: NibConnection("input")},
    ),
    (
      {node: NodeConnection("less-than"), nib: NibConnection("right")},
      {node: NodeConnection("one2"), nib: ValueConnection},
    ),
    (
      {node: NodeConnection("branch"), nib: NibConnection("else")},
      {node: NodeConnection("times"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("times"), nib: NibConnection("left")},
      {node: GraphConnection, nib: NibConnection("input")},
    ),
    (
      {node: NodeConnection("times"), nib: NibConnection("right")},
      {node: NodeConnection("factorial"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("factorial"), nib: NibConnection("input")},
      {node: NodeConnection("minus"), nib: NibConnection("result")},
    ),
    (
      {node: NodeConnection("minus"), nib: NibConnection("left")},
      {node: GraphConnection, nib: NibConnection("input")},
    ),
    (
      {node: NodeConnection("minus"), nib: NibConnection("right")},
      {node: NodeConnection("one3"), nib: ValueConnection},
    ),
  ],
  (),
)

let keyboardEvent = DefinitionMake.f(
  "en",
  ~name="Keyboard Event",
  ~description="From a web browser",
  ~inputs=[("code", "Code")],
  ~implementation=RecordTypeImplementation(
    Belt.Map.String.fromArray([("code", ValueType.PrimitiveValueType(TextType))]),
  ),
  (),
)

let builtins = [
  // Core types
  ("yes", yesLabel),
  ("no", noLabel),
  ("boolean", booleanUnion),
  ("scheduledEventHandler", scheduledEventHandler),
  // References
  ("makeReference", makeReference),
  ("setReference", setReference),
  ("getReference", getReference),
  // Math
  ("plus", plus),
  ("minus", minus),
  ("times", times),
  ("divide", divide),
  ("equals", equals),
  ("less-than", lessThan),
  ("greater-than", greaterThan),
  // Text
  ("concatenateText", concatenateText),
  // flow control
  ("branch", branch),
  ("prerequisite", prerequisite),
  ("log", log),
  // Web view stuff
  ("keyboardEvent", keyboardEvent),
  ("keyboardEventListener", keyboardEventListener),
  ("addKeyboardEventListener", addKeyboardEventListener),
  ("htmlCreateElement", htmlCreateElement),
  ("htmlSetCssProperty", htmlSetCssProperty),
  ("scheduleRecurringEvent", scheduleRecurringEvent),
  ("scheduleAnimationFrame", scheduleAnimationFrame),
]

let builtinsMap = Belt.Map.String.fromArray(builtins)

let v = Belt.Map.String.fromArray([
  // wrapper label
  ("something", somethingLabel),
  ("somethingExample", somethingExample),
  // Stuff used in examples
  ("one", one),
  ("point", point),
  ("example-interface", exampleInterface),
  // Examples
  ("example", example),
  ("simple", simple),
  ("point-example", pointExample),
  ("interface-example", interfaceExample),
  ("nested-inline-example", nestedInlineExample),
  ("factorial", factorial),
  ("factorial2", factorial2),
])
