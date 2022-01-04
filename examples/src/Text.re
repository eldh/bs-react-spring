[%bs.raw {|require('./Text.css')|}];

let transX = x => {j|translate3d(0,$(x)px,0)|j};
let items = ["Lorem", "ipsum", "dolor", "sit"];
let getValues = visible => {
  "opacity": visible ? 1 : 0,
  "x": visible ? 0 : 20,
  "height": visible ? 80 : 0,
};
[@react.component]
let make = () => {
  let (visible, setVisible) = React.useState(() => true);

  let (trail, setTrail) =
    Spring.useTrail(
      ~number=items->Belt_List.length,
      ~config={"mass": 5, "tension": 2000, "friction": 200},
      ~from=getValues(!visible),
      ~values=() => getValues(visible),
      (),
    );
  let _ =
    React.useEffect1(
      () => {
        setTrail(. getValues(visible));
        None;
      },
      [|visible|],
    );

  <div className="trails-main" onClick={_ => setVisible(v => !v)}>
    <div>
      {React.array(
         Belt_Array.mapWithIndex(trail, (index, values) =>
           <Spring.Div
             key={items->Belt_List.getExn(index)}
             className="trails-text"
             style={ReactDOM.Style.make(
               ~opacity=values##opacity,
               ~transform=values##x##interpolate(transX),
               (),
             )}>
             <Spring.Div
               style={ReactDOM.Style.make(~height=values##height, ())}>
               {items->Belt_List.getExn(index)->React.string}
             </Spring.Div>
           </Spring.Div>
         ),
       )}
    </div>
  </div>;
};