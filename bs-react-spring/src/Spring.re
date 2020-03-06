/*
 useSpring(Values | () => Values): animatedProps | [animatedProps, set]
 useTrail(Count, Values | () => Values): [animatedProps] | [[animatedProps], set]
 useTransition(Config): [{ item, key, props, state }]
 useKeyframes.[spring | trail](Config | Func | [Configs], initialProps): Func(SlotName)
 useChain([...refs], timeSteps, timeFrame=1000)
  */
[@bs.deriving abstract]
type config = {
  [@bs.optional]
  mass: float,
  [@bs.optional]
  tension: float,
  [@bs.optional]
  friction: float,
  [@bs.optional]
  clamp: bool,
  [@bs.optional]
  precision: float,
  [@bs.optional]
  velocity: float,
  [@bs.optional]
  delay: float,
  [@bs.optional]
  duration: float,
  [@bs.optional]
  easing: float => float,
};

[@bs.module "react-spring"]
external _externalUseSpring: (unit => 'a) => (Js.t({..}), (. 'a) => unit) =
  "useSpring";

[@bs.module "react-spring"]
external _externalUseTrail:
  (int, unit => 'a) => (array(Js.t({..})), (. 'a) => unit) =
  "useTrail";

let toJsObj = (a, b) => Js.Obj.assign(Obj.magic(b), Obj.magic(a));

let constructSpringObj = (~values, ~from, ~config, ()) =>
  switch (config, from) {
  | (None, None) => (() => toJsObj(values(), Js.Obj.empty()))
  | (Some(c), None) => (() => toJsObj(values(), {"config": c}))
  | (None, Some(f)) => (() => toJsObj(values(), {"from": f}))
  | (Some(c), Some(f)) => (
      () => toJsObj(values(), {"from": f, "config": c})
    )
  };

let useSpring = (~values, ~from=?, ~config=?, ()) =>
  _externalUseSpring(constructSpringObj(~values, ~from, ~config, ()));

/**
 * const [items, set] = useState([...])
const transitions = useTransition(items, item => item.key, {
from: { transform: 'translate3d(0,-40px,0)' },
enter: { transform: 'translate3d(0,0px,0)' },
leave: { transform: 'translate3d(0,-40px,0)' },
})
return transitions.map(({ item, props, key }) =>
<animated.div key={key} style={props}>{item.text}</animated.div>
)
 */
module MakeTransition = (Config: {
                           type t;
                           type item;
                         }) => {
  type springConfig = config;

  [@bs.deriving abstract]
  type config = {
    //	obj/fn	Initial (first time) base values, optional (can be null)
    [@bs.optional]
    initial: Config.t,
    //	obj/fn	Base values, optional
    [@bs.optional]
    from: Config.t,
    //	obj/fn/array(obj)	Styles apply for entering elements
    enter: Config.t,
    //	obj/fn/array(obj)	Styles apply for updating elements (you can update the hook itself with new values)
    [@bs.optional]
    update: Config.t,
    //	obj/fn/array(obj)	Styles apply for leaving elements
    leave: Config.t,
    //	number	Delay in ms before the animation starts, adds up for each enter/update and leave
    [@bs.optional]
    trail: int,
    //	bool/fn	If this is true, items going in and out with the same key will be re-used
    [@bs.optional]
    unique: bool,
    //	bool/fn	Used in combination with "unique" and makes entering items start from scratch
    [@bs.optional]
    reset: bool,
    //	fn	Called when objects have disappeared for good
    [@bs.optional]
    onDestroyed: unit => unit,
    [@bs.optional]
    config: springConfig,
  };

  type transitionType = {
    item: Config.item,
    key: string,
    props: Config.t,
  };

  [@bs.module "react-spring"]
  external use:
    (array(Config.item), Config.item => string, config) =>
    array(transitionType) =
    "useTransition";
};

let useTrail = (~number: int, ~from=?, ~config=?, ~values, ()) =>
  _externalUseTrail(number, constructSpringObj(~values, ~from, ~config, ()));

module MakeSpring = (Config: {
                       type t;
                       type interpolate;
                     }) => {
  type values = Config.t;
  type valuesObj = {. "__values": values};
  type valuesFn = unit => valuesObj;

  [@bs.module "react-spring"]
  external useSpring_: valuesFn => ('retVals, (. valuesObj) => unit) =
    "useSpring";
  [@bs.send]
  external interpolate_: (values, Config.interpolate) => string =
    "interpolate";
  let interpolate = interpolate_;
  let use = (~config=?, ~from=?, startValues) => {
    let (retValues, set_) =
      useSpring_(
        constructSpringObj(
          ~values=() => {"__values": startValues},
          ~from,
          ~config,
          (),
        ),
      );

    let set = (~config=?, setValues) => {
      let setInput =
        constructSpringObj(
          ~values=() => {"__values": setValues},
          ~from,
          ~config,
          (),
          (),
        );
      set_(. setInput);
    };
    (retValues##__values, set);
  };
};

module MakeTrail = (Config: {
                      type t;
                      type interpolate;
                    }) => {
  type values = Config.t;
  type valuesObj = {. "__values": values};
  type trailArr = array(valuesObj);
  type valuesFn = unit => valuesObj;

  [@bs.module "react-spring"]
  external useTrail_:
    (int, unit => valuesObj) => (trailArr, (. valuesObj) => unit) =
    "useTrail";

  [@bs.send]
  external interpolate_: (values, Config.interpolate) => string =
    "interpolate";

  let interpolate = interpolate_;

  let use = (~number, ~config=?, ~from=?, startValues) => {
    let (retValues, set_) =
      useTrail_(
        number,
        constructSpringObj(
          ~values=() => {"__values": startValues()},
          ~from,
          ~config,
          (),
        ),
      );

    let setFn = (~config=?, ~from=?, setValues) => {
      let setInput =
        constructSpringObj(
          ~values=() => {"__values": setValues},
          ~config,
          ~from,
          (),
          (),
        );
      set_(. setInput);
    };
    (retValues |> Array.map(t => t##__values): array(values), setFn);
  };
};

module MakeSprings = (Config: {
                        type t;
                        type interpolate;
                      }) => {
  type values = Config.t;
  type valuesObj = Config.t;
  type springsArr = array(valuesObj);
  type valuesFn = unit => valuesObj;

  [@bs.module "react-spring"]
  external useSprings_:
    (int, int => valuesObj) => (springsArr, (. (int => valuesObj)) => unit) =
    "useSprings";

  [@bs.send]
  external interpolate_: ('a, Config.interpolate) => string = "interpolate";

  let interpolate = interpolate_;

  let use = (~number, ~config=?, ~from=?, startValues) => {
    let (retValues, set_) =
      useSprings_(number, i =>
        Obj.magic(
          constructSpringObj(
            ~values=() => startValues(i),
            ~from,
            ~config,
            (),
            (),
          ),
        )
      );

    let setFn = (~config=?, ~from=?, setValues) => {
      let setInput = i =>
        constructSpringObj(
          ~values=() => setValues(i),
          ~config,
          ~from,
          (),
          (),
        );
      Js.log3("setInput", setInput, setInput(0));

      set_(. Obj.magic(setInput));
    };
    (retValues |> Array.map(t => t): array(values), setFn);
  };
};

module Div = {
  type reactElement;
  [@bs.module "react-spring"] [@bs.scope "animated"]
  external externalDiv_: reactElement = "div";
  [@bs.module "react"]
  external externalCreateElement:
    (reactElement, Js.t({..}), React.element) => React.element =
    "createElement";
  type spreadProps;
  [@bs.deriving abstract]
  type jsProps = {
    [@bs.optional]
    className: string,
    [@bs.optional]
    onMouseMove: ReactEvent.Mouse.t => unit,
    [@bs.optional]
    onClick: ReactEvent.Mouse.t => unit,
    [@bs.optional]
    onMouseDown: ReactEvent.Mouse.t => unit,
    [@bs.optional]
    onMouseLeave: unit => unit,
    [@bs.optional]
    style: ReactDOMRe.Style.t,
  };

  [@react.component]
  let make =
      (
        ~className=?,
        ~spreadProps=?,
        ~onMouseMove=?,
        ~onMouseLeave=?,
        ~onClick=?,
        ~onMouseDown=?,
        ~style,
        ~children=React.null,
        (),
      ) => {
    let props =
      Js.Obj.assign(
        switch (spreadProps) {
        | None => Js.Obj.empty()
        | Some(s) => (Obj.magic(s))(.)
        },
        Obj.magic(
          jsProps(
            ~className?,
            ~onMouseMove?,
            ~onMouseLeave?,
            ~onMouseDown?,
            ~onClick?,
            ~style,
            (),
          ),
        ),
      );

    externalCreateElement(externalDiv_, props, children);
  };
};