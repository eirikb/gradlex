echo "Welcome to gradlex (stage 1)!"

if [[ -f "$USER_PWD/.gradle/bin/gradle" ]]; then
  echo "Gradle already installed, executing..."
  cd "$USER_PWD"
  ./.gradle/bin/gradle
  exit
fi

for gg in gg*
do
  echo "Running $gg..."
  chmod +x "$gg"
  "./$gg" 2> /dev/null && mv gradlex2* gradlex2 && sh gradlex2 -- "$USER_PWD" $1 && echo "Yeah!" && exit
done