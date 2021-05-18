echo "Welcome to gradlex (stage 2)!"
echo "Installing gradle (7.0)...."
cp -r gradle "$1/.gradle/"
echo "Done! All is well"
echo "You should now have gradle-7.0 installed in .gradle"
echo "Press enter to start gradle:"
read
cd "$1"
chmod +x ./.gradle/bin/gradle
./.gradle/bin/gradle $2
