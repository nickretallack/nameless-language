type point = {
  x: float,
  y: float,
};

let sortBy: 'a. (Belt.List.t('a), 'a => 'b) => Belt.List.t('a) =
  (list, func) =>
    Belt.List.map(
      Belt.List.sort(
        Belt.List.map(list, item => (func(item), item)),
        compare,
      ),
      snd,
    );

let rec findIndexExn: 'a. (Belt.List.t('a), 'a) => int =
  (list, needle) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => head == needle ? 0 : 1 + findIndexExn(rest, needle)
    };

let rec findByIndexExn: 'a. (Belt.List.t('a), 'a => bool) => int =
  (list, check) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => check(head) ? 0 : 1 + findByIndexExn(rest, check)
    };

let moveToListIndex: 'a. (list('a), 'a, int) => list('a) =
  (list, needle, newIndex) =>
    Belt.List.reduceWithIndex(list, [], (newList, item, index) =>
      if (item == needle) {
        newList;
      } else if (index == newIndex) {
        List.append(newList, [needle, item]);
      } else {
        List.append(newList, [item]);
      }
    );

let removeElementFromList = (list, element) =>
  Belt.List.keep(list, listElement => element != listElement);

let moveToListIndex: 'a. (list('a), 'a, int) => list('a) =
  (list, needle, newIndex) => {
    let oldIndex = findIndexExn(list, needle);
    let (left, right) =
      switch (
        Belt.List.splitAt(list, newIndex + (newIndex > oldIndex ? 1 : 0))
      ) {
      | None => raise(Not_found)
      | Some(tuple) => tuple
      };
    Belt.List.concat(
      removeElementFromList(left, needle),
      [needle, ...removeElementFromList(right, needle)],
    );
  };

let simpleMergeMaps:
  'a.
  (Belt.Map.String.t('a), Belt.Map.String.t('a)) => Belt.Map.String.t('a)
 =
  (map1, map2) =>
    Belt.Map.String.reduce(map1, map2, (acc, key, value) =>
      Belt.Map.String.set(acc, key, value)
    );

let arrayGetWithDefault: 'a. (array('a), int, 'a) => 'a =
  (array, index, default) =>
    switch (Belt.Array.get(array, index)) {
    | None => default
    | Some(value) => value
    };

Random.init(int_of_float(Js.Date.now()));

let randomHex = () => Printf.sprintf("%x", Random.int(16));
let randomID = () =>
  String.concat("", Belt.List.makeBy(32, _ => randomHex()));

let floatToString = (x: float) => Printf.sprintf("%g", x);
let pixels = (x: float) => Printf.sprintf("%gpx", x);

let pointFromMouse = event => {
  x:
    float_of_int(ReactEvent.Mouse.pageX(event))
    -.
    ReactEvent.Mouse.currentTarget(event)##offsetLeft,

  y:
    float_of_int(ReactEvent.Mouse.pageY(event))
    -.
    ReactEvent.Mouse.currentTarget(event)##offsetTop,
};

let positionStyle = (position: point) =>
  ReactDOMRe.Style.make(
    ~left=pixels(position.x),
    ~top=pixels(position.y),
    (),
  );

external convertToList: Js.t('a) => array('b) = "%identity";

let iterateTouches = (event, callback) =>
  Array.iter(
    callback,
    convertToList(ReactEvent.Touch.changedTouches(event)),
  );

let getEventValue = event => ReactEvent.Form.target(event)##value;

let renderMap = (fn, map) =>
  ReasonReact.array(Array.map(fn, Belt.Map.toArray(map)));
let renderStringMap = (fn, map) =>
  ReasonReact.array(Array.map(fn, Belt.Map.String.toArray(map)));
let renderList = (fn, list) =>
  ReasonReact.array(Array.of_list(List.mapi(fn, list)));
let renderArray = (fn, array) => ReasonReact.array(Array.map(fn, array));
