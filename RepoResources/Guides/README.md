# Guides

When I have more time I'll write up full guides to help you play and modify the game, but for now I'll give brief tips/where to look

---

### Tips for playing
- Play on easy mode until you get used to the controls and terrain, debris is much less of a concern on this game mode
- Sometimes it can help to tilt your car clock wise (D or L key), especially when airborne.
This is caused by the counter clock wise torque that is applied when accelerating forwards.
- The faster you're moving the less likely you're to get stuck on terrain or debris, so keep moving!

---

### Tips for modding
I won't run over all the specifics but the variable names and comments alone should be able to guide you.
For now, I'll just point to where you need look in order to modify different aspects of the game but detailed guides will come at some point.

**That being said I wouldn't advise messing around with the code unless you have an understanding of C++ and Box2D.**
Chances are you'll just break something on your own pull down or destabilise the game physics (poorly weighted car, too heavy boulders, etc).

The documentation for Box2D is found here : https://box2d.org/documentation/

- To change a game mode's difficulty go to 'GameControl/configGameMode.cpp', easiest way to is to change the values of your least favourite game mode
- To modify game entities physics settings go to 'GameEntities/' you'll find multiple .cpp files that define entity physics settings inside of the constructors
- To add new terrain go to 'GameControl/' and check out the 'Chunk' and 'GameManager' classes.
- To change game colours go to 'Visuals/visualColours.h'


**Just note any modifications that you make, you are making of your own accord and at your own risk, and I will not be held responsible (Basically the bold section of the MIT license that I'm licensing my work under, see below)**

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
