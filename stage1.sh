echo "Welcome to gradlex (stage 1)!"

if [[ -f "$USER_PWD/.gradle/bin/gradle" ]]; then
  echo "Gradle already installed, executing..."
  cd "$USER_PWD"
  ./.gradle/bin/gradle
  exit
fi

for gg in gg_ubuntu-latest_x86 gg*
do
  echo "Running $gg..."
  chmod +x "$gg"
  "./$gg" 2> /dev/null && sh gradlex2 -- "$USER_PWD" $1 && exit
done
