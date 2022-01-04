module Index = {
  let className = Css.([marginRight(`px(10))] |> style);
  [@react.component]
  let make = () => {
    <>
      {["card", "pager", "goo", "numbers", "pull", "text"]
       |> List.map(url => {
            <div key=url className>
              <a href={"/" ++ url}> url->React.string </a>
            </div>
          })
       |> Array.of_list
       |> React.array}
    </>;
  };
};

[@react.component]
let make = () => {
  let url = RescriptReactRouter.useUrl();
  Js.log2(url, url.path);
  
  switch (url.path) {
  | ["card"] => <Card />
  | ["pager"] => <ViewPager />
  | ["numbers"] => <Numbers />
  | ["goo"] => <Goo />
  | ["pull"] => <Pull />
  | ["text"] => <Text />
  | _ => <Index />
  };
};
