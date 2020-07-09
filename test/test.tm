*TINY Compilation to TM Code
*File: test.tm
*Standard prelude:
  0:     LD  5,0(0) 	load gp with maxaddress
  1:    LDA  6,-256(5) 	copy gp to mp
  2:     ST  0,0(0) 	clear location 0
*End of standard prelude.
*-> var. decl.
*<- var. decl.
*-> Function (minloc)
  4:     ST  1,-12(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
  3:    LDC  1,6(0) 	func: load function location
  6:     ST  0,-1(6) 	func: store return address
*-> param
*a
*<- param
*-> param
*low
*<- param
*-> param
*high
*<- param
*->compound
*-> assign
*-> Id (k)
  7:    LDC  0,-7(0) 	id: load varOffset
  8:     ADD   0,6,0	id: calculate the address found in current frame
  9:    LDA  0,0(0) 	load id address
*<- Id
 10:     ST  0,-8(6) 	assign: push left (address)
*-> Id (low)
 11:    LDC  0,-3(0) 	id: load varOffset
 12:     ADD   0,6,0	id: calculate the address found in current frame
 13:     LD  0,0(0) 	load id value
*<- Id
 14:     LD  1,-8(6) 	assign: load left (address)
 15:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (j)
 16:    LDC  0,-6(0) 	id: load varOffset
 17:     ADD   0,6,0	id: calculate the address found in current frame
 18:    LDA  0,0(0) 	load id address
*<- Id
 19:     ST  0,-8(6) 	assign: push left (address)
*-> Id (a)
 20:    LDC  0,-2(0) 	id: load varOffset
 21:     ADD   0,6,0	id: load the memory address of base address of array to ac
 22:      LD   0,0,0	id: load the base address of array to ac
 23:     ST  0,-9(6) 	id: push base address
*-> Id (low)
 24:    LDC  0,-3(0) 	id: load varOffset
 25:     ADD   0,6,0	id: calculate the address found in current frame
 26:     LD  0,0(0) 	load id value
*<- Id
 27:     LD  1,-9(6) 	id: pop base address
 28:     SUB   0,1,0	id: calculate element address with index
 29:     LD  0,0(0) 	load id value
*<- Id
 30:     LD  1,-8(6) 	assign: load left (address)
 31:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (i)
 32:    LDC  0,-5(0) 	id: load varOffset
 33:     ADD   0,6,0	id: calculate the address found in current frame
 34:    LDA  0,0(0) 	load id address
*<- Id
 35:     ST  0,-8(6) 	assign: push left (address)
*-> Op
*-> Id (low)
 36:    LDC  0,-3(0) 	id: load varOffset
 37:     ADD   0,6,0	id: calculate the address found in current frame
 38:     LD  0,0(0) 	load id value
*<- Id
 39:     ST  0,-9(6) 	op: push left
*-> Const
 40:    LDC  0,1(0) 	load const
*<- Const
 41:     LD  1,-9(6) 	op: load left
 42:     ADD   0,1,0	op +
*<- Op
 43:     LD  1,-8(6) 	assign: load left (address)
 44:     ST  0,0(1) 	assign: store value
*<- assign
*-> iter.
*while: jump after body comes back here
*-> Op
*-> Id (i)
 45:    LDC  0,-5(0) 	id: load varOffset
 46:     ADD   0,6,0	id: calculate the address found in current frame
 47:     LD  0,0(0) 	load id value
*<- Id
 48:     ST  0,-8(6) 	op: push left
*-> Id (high)
 49:    LDC  0,-4(0) 	id: load varOffset
 50:     ADD   0,6,0	id: calculate the address found in current frame
 51:     LD  0,0(0) 	load id value
*<- Id
 52:     LD  1,-8(6) 	op: load left
 53:     SUB   0,1,0	op <
 54:    JLT  0,2(7) 	br if true
 55:    LDC  0,0(0) 	false case
 56:    LDA  7,1(7) 	unconditional jmp
 57:    LDC  0,1(0) 	true case
*<- Op
*while: jump to end belongs here
*->compound
*->if
*-> Op
*-> Id (a)
 59:    LDC  0,-2(0) 	id: load varOffset
 60:     ADD   0,6,0	id: load the memory address of base address of array to ac
 61:      LD   0,0,0	id: load the base address of array to ac
 62:     ST  0,-8(6) 	id: push base address
*-> Id (i)
 63:    LDC  0,-5(0) 	id: load varOffset
 64:     ADD   0,6,0	id: calculate the address found in current frame
 65:     LD  0,0(0) 	load id value
*<- Id
 66:     LD  1,-8(6) 	id: pop base address
 67:     SUB   0,1,0	id: calculate element address with index
 68:     LD  0,0(0) 	load id value
*<- Id
 69:     ST  0,-8(6) 	op: push left
*-> Id (j)
 70:    LDC  0,-6(0) 	id: load varOffset
 71:     ADD   0,6,0	id: calculate the address found in current frame
 72:     LD  0,0(0) 	load id value
*<- Id
 73:     LD  1,-8(6) 	op: load left
 74:     SUB   0,1,0	op <
 75:    JLT  0,2(7) 	br if true
 76:    LDC  0,0(0) 	false case
 77:    LDA  7,1(7) 	unconditional jmp
 78:    LDC  0,1(0) 	true case
*<- Op
*if:jump to else belongs here
*->compound
*-> assign
*-> Id (j)
 80:    LDC  0,-6(0) 	id: load varOffset
 81:     ADD   0,6,0	id: calculate the address found in current frame
 82:    LDA  0,0(0) 	load id address
*<- Id
 83:     ST  0,-8(6) 	assign: push left (address)
*-> Id (a)
 84:    LDC  0,-2(0) 	id: load varOffset
 85:     ADD   0,6,0	id: load the memory address of base address of array to ac
 86:      LD   0,0,0	id: load the base address of array to ac
 87:     ST  0,-9(6) 	id: push base address
*-> Id (i)
 88:    LDC  0,-5(0) 	id: load varOffset
 89:     ADD   0,6,0	id: calculate the address found in current frame
 90:     LD  0,0(0) 	load id value
*<- Id
 91:     LD  1,-9(6) 	id: pop base address
 92:     SUB   0,1,0	id: calculate element address with index
 93:     LD  0,0(0) 	load id value
*<- Id
 94:     LD  1,-8(6) 	assign: load left (address)
 95:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (k)
 96:    LDC  0,-7(0) 	id: load varOffset
 97:     ADD   0,6,0	id: calculate the address found in current frame
 98:    LDA  0,0(0) 	load id address
*<- Id
 99:     ST  0,-8(6) 	assign: push left (address)
*-> Id (i)
100:    LDC  0,-5(0) 	id: load varOffset
101:     ADD   0,6,0	id: calculate the address found in current frame
102:     LD  0,0(0) 	load id value
*<- Id
103:     LD  1,-8(6) 	assign: load left (address)
104:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
*if:jump to end belongs here
 79:     JEQ 0,26(7)	if: jmp to else
105:     LDA 7,0(7)	jmp to end
*<- if
*-> assign
*-> Id (i)
106:    LDC  0,-5(0) 	id: load varOffset
107:     ADD   0,6,0	id: calculate the address found in current frame
108:    LDA  0,0(0) 	load id address
*<- Id
109:     ST  0,-8(6) 	assign: push left (address)
*-> Op
*-> Id (i)
110:    LDC  0,-5(0) 	id: load varOffset
111:     ADD   0,6,0	id: calculate the address found in current frame
112:     LD  0,0(0) 	load id value
*<- Id
113:     ST  0,-9(6) 	op: push left
*-> Const
114:    LDC  0,1(0) 	load const
*<- Const
115:     LD  1,-9(6) 	op: load left
116:     ADD   0,1,0	op +
*<- Op
117:     LD  1,-8(6) 	assign: load left (address)
118:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
119:     LDA 7,-75(7)	while: jmp back to test
 58:     JEQ 0,61(7)	while: jmp to end
*<- iter.
*-> return
*-> Id (k)
120:    LDC  0,-7(0) 	id: load varOffset
121:     ADD   0,6,0	id: calculate the address found in current frame
122:     LD  0,0(0) 	load id value
*<- Id
123:     LD  7,-1(6) 	return: to caller
*<- return
*<-compound
124:     LD  7,-1(6) 	func: load pc with return address
  5:     LDA 7,119(7)	func: unconditional jump to next declaration
* frameoffset (-11) 
*-> Function (sort)
126:     ST  1,-13(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
125:    LDC  1,128(0) 	func: load function location
128:     ST  0,-1(6) 	func: store return address
*-> param
*a
*<- param
*-> param
*low
*<- param
*-> param
*high
*<- param
*->compound
*-> assign
*-> Id (i)
129:    LDC  0,-5(0) 	id: load varOffset
130:     ADD   0,6,0	id: calculate the address found in current frame
131:    LDA  0,0(0) 	load id address
*<- Id
132:     ST  0,-8(6) 	assign: push left (address)
*-> Id (low)
133:    LDC  0,-3(0) 	id: load varOffset
134:     ADD   0,6,0	id: calculate the address found in current frame
135:     LD  0,0(0) 	load id value
*<- Id
136:     LD  1,-8(6) 	assign: load left (address)
137:     ST  0,0(1) 	assign: store value
*<- assign
*-> iter.
*while: jump after body comes back here
*-> Op
*-> Id (i)
138:    LDC  0,-5(0) 	id: load varOffset
139:     ADD   0,6,0	id: calculate the address found in current frame
140:     LD  0,0(0) 	load id value
*<- Id
141:     ST  0,-8(6) 	op: push left
*-> Op
*-> Id (high)
142:    LDC  0,-4(0) 	id: load varOffset
143:     ADD   0,6,0	id: calculate the address found in current frame
144:     LD  0,0(0) 	load id value
*<- Id
145:     ST  0,-9(6) 	op: push left
*-> Const
146:    LDC  0,1(0) 	load const
*<- Const
147:     LD  1,-9(6) 	op: load left
148:     SUB   0,1,0	op -
*<- Op
149:     LD  1,-8(6) 	op: load left
150:     SUB   0,1,0	op <
151:    JLT  0,2(7) 	br if true
152:    LDC  0,0(0) 	false case
153:    LDA  7,1(7) 	unconditional jmp
154:    LDC  0,1(0) 	true case
*<- Op
*while: jump to end belongs here
*->compound
*-> assign
*-> Id (k)
156:    LDC  0,-6(0) 	id: load varOffset
157:     ADD   0,6,0	id: calculate the address found in current frame
158:    LDA  0,0(0) 	load id address
*<- Id
159:     ST  0,-8(6) 	assign: push left (address)
*-> Call
*-> Id (a)
160:    LDC  0,-2(0) 	id: load varOffset
161:     ADD   0,6,0	id: load the memory address of base address of array to ac
162:      LD   0,0,0	id: load the base address of array to ac
163:     ST  0,-9(6) 	id: push base address
164:     LD  1,-9(6) 	id: pop base address
165:    LDA  0,0(0) 	load id address
*<- Id
166:     ST  0,-11(6) 	call: push argument
*-> Id (i)
167:    LDC  0,-5(0) 	id: load varOffset
168:     ADD   0,6,0	id: calculate the address found in current frame
169:     LD  0,0(0) 	load id value
*<- Id
170:     ST  0,-12(6) 	call: push argument
*-> Id (high)
171:    LDC  0,-4(0) 	id: load varOffset
172:     ADD   0,6,0	id: calculate the address found in current frame
173:     LD  0,0(0) 	load id value
*<- Id
174:     ST  0,-13(6) 	call: push argument
175:     ST  6,-9(6) 	call: store current mp
176:    LDA  6,-9(6) 	call: push new frame
177:    LDA  0,1(7) 	call: save return in ac
178:     LD  7,-12(5) 	call: relative jump to function entry
179:     LD  6,0(6) 	call: pop current frame
*<- Call
180:     LD  1,-8(6) 	assign: load left (address)
181:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (t)
182:    LDC  0,-7(0) 	id: load varOffset
183:     ADD   0,6,0	id: calculate the address found in current frame
184:    LDA  0,0(0) 	load id address
*<- Id
185:     ST  0,-8(6) 	assign: push left (address)
*-> Id (a)
186:    LDC  0,-2(0) 	id: load varOffset
187:     ADD   0,6,0	id: load the memory address of base address of array to ac
188:      LD   0,0,0	id: load the base address of array to ac
189:     ST  0,-9(6) 	id: push base address
*-> Id (k)
190:    LDC  0,-6(0) 	id: load varOffset
191:     ADD   0,6,0	id: calculate the address found in current frame
192:     LD  0,0(0) 	load id value
*<- Id
193:     LD  1,-9(6) 	id: pop base address
194:     SUB   0,1,0	id: calculate element address with index
195:     LD  0,0(0) 	load id value
*<- Id
196:     LD  1,-8(6) 	assign: load left (address)
197:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (a)
198:    LDC  0,-2(0) 	id: load varOffset
199:     ADD   0,6,0	id: load the memory address of base address of array to ac
200:      LD   0,0,0	id: load the base address of array to ac
201:     ST  0,-8(6) 	id: push base address
*-> Id (k)
202:    LDC  0,-6(0) 	id: load varOffset
203:     ADD   0,6,0	id: calculate the address found in current frame
204:     LD  0,0(0) 	load id value
*<- Id
205:     LD  1,-8(6) 	id: pop base address
206:     SUB   0,1,0	id: calculate element address with index
207:    LDA  0,0(0) 	load id address
*<- Id
208:     ST  0,-8(6) 	assign: push left (address)
*-> Id (a)
209:    LDC  0,-2(0) 	id: load varOffset
210:     ADD   0,6,0	id: load the memory address of base address of array to ac
211:      LD   0,0,0	id: load the base address of array to ac
212:     ST  0,-9(6) 	id: push base address
*-> Id (i)
213:    LDC  0,-5(0) 	id: load varOffset
214:     ADD   0,6,0	id: calculate the address found in current frame
215:     LD  0,0(0) 	load id value
*<- Id
216:     LD  1,-9(6) 	id: pop base address
217:     SUB   0,1,0	id: calculate element address with index
218:     LD  0,0(0) 	load id value
*<- Id
219:     LD  1,-8(6) 	assign: load left (address)
220:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (a)
221:    LDC  0,-2(0) 	id: load varOffset
222:     ADD   0,6,0	id: load the memory address of base address of array to ac
223:      LD   0,0,0	id: load the base address of array to ac
224:     ST  0,-8(6) 	id: push base address
*-> Id (i)
225:    LDC  0,-5(0) 	id: load varOffset
226:     ADD   0,6,0	id: calculate the address found in current frame
227:     LD  0,0(0) 	load id value
*<- Id
228:     LD  1,-8(6) 	id: pop base address
229:     SUB   0,1,0	id: calculate element address with index
230:    LDA  0,0(0) 	load id address
*<- Id
231:     ST  0,-8(6) 	assign: push left (address)
*-> Id (t)
232:    LDC  0,-7(0) 	id: load varOffset
233:     ADD   0,6,0	id: calculate the address found in current frame
234:     LD  0,0(0) 	load id value
*<- Id
235:     LD  1,-8(6) 	assign: load left (address)
236:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (i)
237:    LDC  0,-5(0) 	id: load varOffset
238:     ADD   0,6,0	id: calculate the address found in current frame
239:    LDA  0,0(0) 	load id address
*<- Id
240:     ST  0,-8(6) 	assign: push left (address)
*-> Op
*-> Id (i)
241:    LDC  0,-5(0) 	id: load varOffset
242:     ADD   0,6,0	id: calculate the address found in current frame
243:     LD  0,0(0) 	load id value
*<- Id
244:     ST  0,-9(6) 	op: push left
*-> Const
245:    LDC  0,1(0) 	load const
*<- Const
246:     LD  1,-9(6) 	op: load left
247:     ADD   0,1,0	op +
*<- Op
248:     LD  1,-8(6) 	assign: load left (address)
249:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
250:     LDA 7,-113(7)	while: jmp back to test
155:     JEQ 0,95(7)	while: jmp to end
*<- iter.
*<-compound
251:     LD  7,-1(6) 	func: load pc with return address
127:     LDA 7,124(7)	func: unconditional jump to next declaration
* frameoffset (-11) 
*-> Function (main)
253:     ST  1,-14(5) 	func: store the location of func. entry
*func: unconditional jump to next declaration belongs here
*func: function body starts here
252:    LDC  1,255(0) 	func: load function location
255:     ST  0,-1(6) 	func: store return address
*->compound
*-> assign
*-> Id (i)
256:    LDC  0,-2(0) 	id: load varOffset
257:     ADD   0,6,0	id: calculate the address found in current frame
258:    LDA  0,0(0) 	load id address
*<- Id
259:     ST  0,-3(6) 	assign: push left (address)
*-> Const
260:    LDC  0,0(0) 	load const
*<- Const
261:     LD  1,-3(6) 	assign: load left (address)
262:     ST  0,0(1) 	assign: store value
*<- assign
*-> iter.
*while: jump after body comes back here
*-> Op
*-> Id (i)
263:    LDC  0,-2(0) 	id: load varOffset
264:     ADD   0,6,0	id: calculate the address found in current frame
265:     LD  0,0(0) 	load id value
*<- Id
266:     ST  0,-3(6) 	op: push left
*-> Const
267:    LDC  0,10(0) 	load const
*<- Const
268:     LD  1,-3(6) 	op: load left
269:     SUB   0,1,0	op <
270:    JLT  0,2(7) 	br if true
271:    LDC  0,0(0) 	false case
272:    LDA  7,1(7) 	unconditional jmp
273:    LDC  0,1(0) 	true case
*<- Op
*while: jump to end belongs here
*->compound
*-> assign
*-> Id (x)
275:    LDC  0,-2(0) 	id: load varOffset
276:     ADD   0,5,0	id: calculate the address
277:     ST  0,-3(6) 	id: push base address
*-> Id (i)
278:    LDC  0,-2(0) 	id: load varOffset
279:     ADD   0,6,0	id: calculate the address found in current frame
280:     LD  0,0(0) 	load id value
*<- Id
281:     LD  1,-3(6) 	id: pop base address
282:     SUB   0,1,0	id: calculate element address with index
283:    LDA  0,0(0) 	load id address
*<- Id
284:     ST  0,-3(6) 	assign: push left (address)
*-> Call
285:      IN   0,0,0	read integer value
*<- Call
286:     LD  1,-3(6) 	assign: load left (address)
287:     ST  0,0(1) 	assign: store value
*<- assign
*-> assign
*-> Id (i)
288:    LDC  0,-2(0) 	id: load varOffset
289:     ADD   0,6,0	id: calculate the address found in current frame
290:    LDA  0,0(0) 	load id address
*<- Id
291:     ST  0,-3(6) 	assign: push left (address)
*-> Op
*-> Id (i)
292:    LDC  0,-2(0) 	id: load varOffset
293:     ADD   0,6,0	id: calculate the address found in current frame
294:     LD  0,0(0) 	load id value
*<- Id
295:     ST  0,-4(6) 	op: push left
*-> Const
296:    LDC  0,1(0) 	load const
*<- Const
297:     LD  1,-4(6) 	op: load left
298:     ADD   0,1,0	op +
*<- Op
299:     LD  1,-3(6) 	assign: load left (address)
300:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
301:     LDA 7,-39(7)	while: jmp back to test
274:     JEQ 0,27(7)	while: jmp to end
*<- iter.
*-> Call
*-> Id (x)
302:    LDC  0,-2(0) 	id: load varOffset
303:     ADD   0,5,0	id: calculate the address
304:     ST  0,-3(6) 	id: push base address
305:     LD  1,-3(6) 	id: pop base address
306:    LDA  0,0(0) 	load id address
*<- Id
307:     ST  0,-5(6) 	call: push argument
*-> Const
308:    LDC  0,0(0) 	load const
*<- Const
309:     ST  0,-6(6) 	call: push argument
*-> Const
310:    LDC  0,10(0) 	load const
*<- Const
311:     ST  0,-7(6) 	call: push argument
312:     ST  6,-3(6) 	call: store current mp
313:    LDA  6,-3(6) 	call: push new frame
314:    LDA  0,1(7) 	call: save return in ac
315:     LD  7,-13(5) 	call: relative jump to function entry
316:     LD  6,0(6) 	call: pop current frame
*<- Call
*-> assign
*-> Id (i)
317:    LDC  0,-2(0) 	id: load varOffset
318:     ADD   0,6,0	id: calculate the address found in current frame
319:    LDA  0,0(0) 	load id address
*<- Id
320:     ST  0,-3(6) 	assign: push left (address)
*-> Const
321:    LDC  0,0(0) 	load const
*<- Const
322:     LD  1,-3(6) 	assign: load left (address)
323:     ST  0,0(1) 	assign: store value
*<- assign
*-> iter.
*while: jump after body comes back here
*-> Op
*-> Id (i)
324:    LDC  0,-2(0) 	id: load varOffset
325:     ADD   0,6,0	id: calculate the address found in current frame
326:     LD  0,0(0) 	load id value
*<- Id
327:     ST  0,-3(6) 	op: push left
*-> Const
328:    LDC  0,10(0) 	load const
*<- Const
329:     LD  1,-3(6) 	op: load left
330:     SUB   0,1,0	op <
331:    JLT  0,2(7) 	br if true
332:    LDC  0,0(0) 	false case
333:    LDA  7,1(7) 	unconditional jmp
334:    LDC  0,1(0) 	true case
*<- Op
*while: jump to end belongs here
*->compound
*-> Call
*-> Id (x)
336:    LDC  0,-2(0) 	id: load varOffset
337:     ADD   0,5,0	id: calculate the address
338:     ST  0,-3(6) 	id: push base address
*-> Id (i)
339:    LDC  0,-2(0) 	id: load varOffset
340:     ADD   0,6,0	id: calculate the address found in current frame
341:     LD  0,0(0) 	load id value
*<- Id
342:     LD  1,-3(6) 	id: pop base address
343:     SUB   0,1,0	id: calculate element address with index
344:     LD  0,0(0) 	load id value
*<- Id
345:     ST  0,-5(6) 	call: push argument
346:     LD  0,-5(6) 	load arg to ac
347:     OUT   0,0,0	write ac
*<- Call
*-> assign
*-> Id (i)
348:    LDC  0,-2(0) 	id: load varOffset
349:     ADD   0,6,0	id: calculate the address found in current frame
350:    LDA  0,0(0) 	load id address
*<- Id
351:     ST  0,-3(6) 	assign: push left (address)
*-> Op
*-> Id (i)
352:    LDC  0,-2(0) 	id: load varOffset
353:     ADD   0,6,0	id: calculate the address found in current frame
354:     LD  0,0(0) 	load id value
*<- Id
355:     ST  0,-4(6) 	op: push left
*-> Const
356:    LDC  0,1(0) 	load const
*<- Const
357:     LD  1,-4(6) 	op: load left
358:     ADD   0,1,0	op +
*<- Op
359:     LD  1,-3(6) 	assign: load left (address)
360:     ST  0,0(1) 	assign: store value
*<- assign
*<-compound
361:     LDA 7,-38(7)	while: jmp back to test
335:     JEQ 0,26(7)	while: jmp to end
*<- iter.
*<-compound
362:     LD  7,-1(6) 	func: load pc with return address
254:     LDA 7,108(7)	func: unconditional jump to next declaration
* frameoffset (-4) 
363:    LDC  0,-13(0) 	init: load globalOffset
364:     ADD   6,6,0	init: initialize mp with globalOffset
*-> Call
365:     ST  6,0(6) 	call: store current mp
366:    LDA  6,0(6) 	call: push new frame
367:    LDA  0,1(7) 	call: save return in ac
368:    LDC  7,255(0) 	call: unconditional jump to main() entry
369:     LD  6,0(6) 	call: pop current frame
*<- Call
*End of execution.
370:    HALT   0,0,0	
