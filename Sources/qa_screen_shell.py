from pathlib import Path
import subprocess,tempfile,os,signal,time
root=Path.cwd()
main=(root/'package/APPS/SonicMania.sh').read_text()
chunk=main[main.index('. "$app_dir/tools/screen-control.sh"'):]
control=(root/'package/APPS/SonicMania_RGDSPlus/tools/screen-control.sh').read_text()
guard=(root/'package/APPS/SonicMania_RGDSPlus/tools/screen-guard.sh').read_text()
for mode in ['normal','error','signal']:
 with tempfile.TemporaryDirectory() as temp:
  d=Path(temp);(d/'tools').mkdir();node=d/'brightness';node.write_text('63\n')
  (d/'tools/screen-control.sh').write_text(control.replace('/sys/class/backlight/backlight/brightness',str(node)))
  (d/'tools/screen-guard.sh').write_text(guard)
  delay='20' if mode=='signal' else '0.1';code='7' if mode=='error' else '0'
  body=f'exec sleep {delay}' if mode=='signal' else f'sleep {delay}\nexit {code}'
  (d/'SonicMania').write_text(f'#!/bin/bash\necho 0 > "{node}"\n{body}\n');(d/'SonicMania').chmod(0o755)
  launcher=d/'run.sh';launcher.write_text('#!/bin/bash\nset -u\napp_dir="'+str(d)+'"\ncd "$app_dir"\n'+chunk)
  p=subprocess.Popen(['bash',str(launcher)],stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
  if mode=='signal':
   for _ in range(100):
    if node.read_text().strip()=='0':break
    time.sleep(.01)
   p.send_signal(signal.SIGTERM)
  out,_=p.communicate(timeout=10)
  assert p.returncode==({'normal':0,'error':7,'signal':143}[mode]),(mode,p.returncode,out)
  assert node.read_text().strip()=='63',(mode,out)
  print('PASS: launcher screen restoration on',mode)
with tempfile.TemporaryDirectory() as temp:
 d=Path(temp);node=d/'brightness';node.write_text('0\n')
 child=subprocess.Popen(['sleep','10'])
 p=subprocess.Popen(['setsid','bash',str(root/'package/APPS/SonicMania_RGDSPlus/tools/screen-guard.sh'),str(child.pid),'44',str(node)])
 child.kill();child.wait();assert p.wait(timeout=3)==0;assert node.read_text().strip()=='44'
 print('PASS: independent guardian restores after forced child death')
