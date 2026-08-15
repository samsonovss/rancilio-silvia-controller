#pragma once

#include <string>

#include "esphome/components/web_server_base/web_server_base.h"

#include "shot_profiles.h"
#include "user_profiles.h"

namespace silvia_web {

static const char COFFEE_DASHBOARD_HTML[] = R"HTML(<!doctype html>
<html lang="ru">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
  <meta name="theme-color" content="#111318">
  <title>Rancilio Silvia</title>
  <style>
@keyframes user-profile-attention{0%,to{transform:translateX(0)}35%{transform:translateX(-3px)}70%{transform:translateX(3px)}}@keyframes settingsHelpReveal{0%{opacity:0;transform:translateY(-2px)}to{opacity:1;transform:translateY(0)}}@keyframes machineMascotBlink{0%,43%,47%,to{transform:scaleY(1)}45%,46%{transform:scaleY(.08)}}@keyframes machineMascotLook{0%,12%,to{transform:translate(0,0)}28%,40%{transform:translate(-2px,0)}58%,70%{transform:translate(2px,1px)}84%{transform:translate(0,-1px)}}@keyframes machineMascotHeatLook{0%,to{transform:translate(0,1px)}50%{transform:translate(0,-2px)}}@keyframes machineMascotWaterLook{0%,to{transform:translate(-2px,1px)}50%{transform:translate(2px,1px)}}@keyframes machineMascotWarm{0%,to{box-shadow:inset 0 1px 0 rgba(255,255,255,.04),0 0 10px rgba(225,139,72,.04)}50%{box-shadow:inset 0 1px 0 rgba(255,255,255,.04),0 0 18px rgba(225,139,72,.11)}}@keyframes machineMascotFocus{0%,to{transform:translateY(0)}50%{transform:translateY(1px)}}@keyframes machineMascotSmile{0%,to{transform:translateX(-50%) scaleX(1)}50%{transform:translateX(-50%) scaleX(1.12)}}@keyframes machineMascotSteam{0%{transform:translateY(5px) scaleY(.45);opacity:0}25%{opacity:.65}to{transform:translateY(-6px) scaleY(1.12);opacity:0}}:root{color-scheme:dark;--bg:#0d0f13;--card:#171a20;--card2:#1d2129;--text:#f1f3f5;--muted:#9299a6;--accent:#e18b48;--blue:#54a9ff;--green:#4bd18b;--red:#ff6868;--purple:#9a72ef;--line:#2a303a;--shadow:0 12px 32px #0005;--sidebar:226px}*{box-sizing:border-box}html{background:var(--bg)}body{margin:0;min-height:100vh;background:radial-gradient(circle at 80% -10%,#302117 0,transparent 34%),var(--bg);color:var(--text);font:14px/1.4 system-ui,-apple-system,Segoe UI,Roboto,sans-serif}.button,button,input,select{font:inherit}.button,button{appearance:none;border:0;border-radius:13px;padding:12px 16px;font-weight:700;color:#fff;background:#303641;cursor:pointer;text-decoration:none;text-align:center}.button:active,button:active{transform:translateY(1px)}button:disabled{opacity:.45;cursor:default;transform:none}button.primary{background:var(--accent);color:#17100b}button.stop{background:#4b2528;color:#ffb6b6}button.small{padding:8px 10px;border-radius:10px;font-size:12px}.shell{width:min(1260px,100%);margin:0 auto;padding:14px;display:grid;grid-template-columns:var(--sidebar) minmax(0,1fr);gap:14px;transition:grid-template-columns .2s ease}body.sidebar-collapsed .shell{grid-template-columns:55px minmax(0,1fr)}.sidebar{position:sticky;top:14px;height:calc(100vh - 28px);min-height:520px;padding:12px;display:flex;flex-direction:column;background:linear-gradient(160deg,#191d24,#11141a);border:1px solid var(--line);border-radius:18px;box-shadow:var(--shadow);overflow:hidden;z-index:20}.side-head{display:flex;align-items:center;gap:10px;min-height:42px}.menu-toggle{width:42px;min-width:42px;height:42px;padding:0}.dashboard-hide-button svg,.menu-toggle svg,.nav-icon svg{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}.side-brand{min-width:0;font-weight:800;font-size:16px;white-space:nowrap}.brand small,.side-brand small{color:var(--muted);font-weight:500}.side-brand small{display:block;font-size:11px}.side-nav{display:grid;gap:6px;margin-top:18px}.nav-item{width:100%;display:flex;align-items:center;gap:11px;padding:11px 12px;background:0 0;color:var(--muted);text-align:left;white-space:nowrap}.nav-item:hover{background:#232832;color:var(--text)}.nav-item.active{background:#2b3039;color:var(--text);box-shadow:none}.nav-icon{width:24px;min-width:24px;height:24px;display:grid;place-items:center}.dashboard-hide-button svg,.nav-icon svg{stroke-width:1.75}.nav-label{overflow:hidden;transition:opacity .15s ease}.side-footer{margin-top:auto;color:var(--muted);font-size:11px;padding:10px 8px 2px;white-space:nowrap}body.sidebar-collapsed .side-brand,body.sidebar-collapsed .side-footer{opacity:0;width:0;pointer-events:none}.backdrop,body.sidebar-collapsed .nav-label{display:none}body.sidebar-collapsed .sidebar{padding:6px 5px;border-radius:15px}body.sidebar-collapsed .side-head{width:100%;justify-content:center;gap:0}body.sidebar-collapsed .menu-toggle{width:43px;min-width:43px;height:43px;display:grid;place-items:center;justify-self:center;margin:0;padding:0;border-radius:0;background:0 0;box-shadow:none;line-height:0}body.sidebar-collapsed .menu-toggle svg{display:block;margin:0}body.sidebar-collapsed .menu-toggle:hover{background:0 0}body.sidebar-collapsed .side-nav{width:100%;margin-top:11px;gap:5px}body.sidebar-collapsed .nav-item{width:43px;min-width:43px;height:43px;min-height:43px;display:grid;grid-template-columns:24px;grid-template-rows:24px;place-content:center;justify-self:center;margin:0;gap:0;padding:0;border-radius:8px;text-align:center;line-height:0}body.sidebar-collapsed .nav-icon{min-width:24px;line-height:0}body.sidebar-collapsed .nav-icon,body.sidebar-collapsed .nav-icon svg{display:block;width:24px;height:24px;margin:0}body.sidebar-collapsed .nav-item.active{background:#2b3039;color:var(--text);box-shadow:none}.content{min-width:0;max-width:1020px;width:100%;justify-self:center;transition:transform .2s ease}@media (min-width:841px){body.sidebar-collapsed .content{transform:translateX(-34.5px)}}header{display:flex;align-items:center}.mobile-menu{display:none}.brand{font-weight:800;letter-spacing:.2px}.brand small{font-size:12px}.header-spacer{flex:1}.online{border:1px solid var(--line);border-radius:999px;color:var(--muted);white-space:nowrap}.online.ok{color:var(--green);border-color:#286746}.page{display:none}.page.active{display:block}.page-heading{margin:2px 0 12px}.history-page-head h1,.page-heading h1,.settings-page-head h1{margin:0;font-size:20px}.page-heading p{margin:3px 0 0;color:var(--muted)}.grid,.hero{display:grid;gap:12px}.hero{grid-template-columns:1.2fr .8fr}.grid{grid-template-columns:repeat(2,minmax(0,1fr))}.shot-board-label{display:flex;align-items:center;min-height:20px;margin:0 2px;color:var(--text);font-size:14px;line-height:1.2;font-weight:400;letter-spacing:.01em;text-transform:none}.card{min-width:0;background:linear-gradient(145deg,var(--card2),var(--card));border:1px solid var(--line);border-radius:18px;padding:16px;box-shadow:var(--shadow)}.pressure{display:flex;align-items:baseline;gap:8px}.pressure strong{font-size:52px;line-height:1;font-weight:760}.metric span,.muted,.unit{color:var(--muted)}.target{margin-top:7px}.target b{color:var(--blue)}.metrics{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:10px}.metric{min-width:0;background:#11141a;border:1px solid var(--line);border-radius:14px;padding:12px}.metric span{display:block;font-size:12px}.metric b{display:block;font-size:22px;margin-top:4px;overflow-wrap:anywhere}.status-row{display:flex;align-items:center;gap:9px;margin-top:14px;flex-wrap:wrap}.pill{padding:6px 10px;border-radius:999px;background:#242a34;color:#cdd3dc}.pill.run{background:#263d31;color:#78e8aa}.actions{display:flex;gap:10px;margin-top:14px}.actions>*{flex:1}.home-layout-toolbar{display:flex;align-items:center;justify-content:flex-end;flex-wrap:wrap;gap:10px;min-height:34px;margin-bottom:10px}.home-layout-hint{margin-right:auto;color:#89929f;font-size:11px;font-weight:650}.home-hidden-blocks[hidden],.home-layout-actions button[hidden],.home-layout-hint[hidden]{display:none}.home-hidden-blocks,.home-layout-actions{display:flex;align-items:center;gap:7px}.home-hidden-blocks{flex:1 0 100%;gap:8px;padding:8px 9px;border:1px dashed #4b5664;border-radius:11px;background:#151920}.home-hidden-blocks>span{color:var(--muted);font-size:10px;font-weight:750;white-space:nowrap}.home-hidden-block-list{display:flex;flex-wrap:wrap;gap:6px}.home-hidden-block-list button{min-height:28px;padding:5px 9px;border-color:#44505e;border-radius:999px;background:#222831;color:#d8dee7;font-size:10px}.home-dashboard-layout{display:grid;grid-template-columns:minmax(0,1fr);align-items:start}.dashboard-block{position:relative;min-width:0;margin-top:0!important;transition:opacity .15s ease,outline-color .15s ease,transform .15s ease}.dashboard-block-wide{grid-column:1/-1}.dashboard-drag-handle,.dashboard-hide-button{display:none;position:absolute;top:-11px;height:25px;border-color:#596574;border-radius:999px;background:#262d36;box-shadow:0 5px 16px rgba(0,0,0,.35)}.dashboard-drag-handle{left:50%;z-index:12;transform:translateX(-50%);min-width:42px;padding:0 10px;color:#d8dee7;font-size:17px;line-height:1;cursor:grab;touch-action:none;user-select:none}.dashboard-hide-button{right:8px;z-index:13;width:31px;min-width:31px;padding:0;color:#c8d0da}.dashboard-hide-button svg{width:16px;height:16px;stroke-width:1.8}.home-layout-editing .dashboard-block{margin-top:8px!important;outline:1px dashed #596574;outline-offset:3px;border-radius:16px}.home-layout-editing .dashboard-drag-handle,.home-layout-editing .dashboard-hide-button{display:inline-grid;place-items:center}.dashboard-drag-handle:active{cursor:grabbing;transform:translateX(-50%) scale(.96)}.home-layout-editing .dashboard-block>:not(.dashboard-drag-handle):not(.dashboard-hide-button){pointer-events:none}.dashboard-block.dashboard-dragging{opacity:.38;transform:scale(.99);pointer-events:none}.dashboard-block.dashboard-drop-target{outline-color:#e18b48}body.dashboard-layout-dragging{cursor:grabbing;user-select:none}.home-overview{display:grid}.home-top-grid{display:grid;grid-template-columns:minmax(430px,3fr) minmax(330px,2fr)}.machine-head-actions{display:flex;align-items:center;gap:11px}.machine-card,.power-inline{display:flex;justify-content:center}.power-inline{align-items:center;gap:6px;min-width:94px;padding:8px 10px;border:1px solid #68343a;border-radius:11px;background:linear-gradient(145deg,#3b2024,#25171a);color:#faa;font-size:11px;font-weight:750;box-shadow:none}.power-inline.on{background:linear-gradient(145deg,#183a2a,#12271e);color:#9bf0bd;border-color:#2c6848}.power-inline .power-symbol{font-size:18px;line-height:1}.machine-card{flex-direction:column;min-height:100%}.machine-card-head{display:flex;justify-content:space-between;gap:12px}.machine-name{font-size:18px;font-weight:800}.machine-state{margin-top:3px;color:var(--muted);font-size:14px}.machine-state.ready{color:var(--green)}.machine-state.busy{color:var(--accent)}.machine-state.error{color:var(--red)}.machine-reading{min-width:0}.machine-summary{display:flex;align-items:center;gap:9px;flex-wrap:wrap}.machine-summary b{font-size:20px}.machine-auto-off,.machine-temperature{white-space:nowrap}.machine-auto-off{display:inline-flex;align-items:center;gap:6px;min-height:27px;padding:4px 9px;border:1px solid #343c47;border-radius:9px;background:#151a20;color:#aab3bf;line-height:1}.machine-auto-off[hidden]{display:none}.machine-auto-off-icon{flex:0 0 auto;color:#aab3bf;font-size:16px;line-height:1}.machine-auto-off b{color:#d7dce3;font-size:13px;font-weight:800;white-space:nowrap}.machine-auto-off.warn{border-color:#69572d;background:#211d14;color:#ffd166}.machine-auto-off.warn .machine-auto-off-icon,.machine-auto-off.warn b{color:#ffd166}.home-profile-picker{position:relative;display:inline-flex;align-items:center;gap:7px;min-height:32px;margin-left:4px;padding:5px 9px 5px 11px;border:1px solid #39414d;border-radius:999px;background:linear-gradient(145deg,#252b34,#1b2027);box-shadow:inset 0 1px 0 rgba(255,255,255,.045),0 4px 12px rgba(0,0,0,.16);color:var(--muted);transition:border-color .18s ease,background .18s ease,box-shadow .18s ease}.home-profile-picker:hover{border-color:#566171;background:linear-gradient(145deg,#2b323c,#20262e)}.home-profile-picker:focus-within{border-color:#9b673e;box-shadow:0 0 0 3px rgba(225,139,72,.12),inset 0 1px 0 rgba(255,255,255,.05)}.home-profile-picker>span{font-size:10px;line-height:1;font-weight:700;letter-spacing:.035em;text-transform:uppercase}.home-profile-picker::after{content:"⌄";position:absolute;right:9px;top:50%;transform:translateY(-58%);color:var(--accent);font-size:15px;line-height:1;pointer-events:none}.home-profile-picker select{width:auto;min-width:112px;max-width:165px;padding:0 21px 0 0;border:0;border-radius:0;outline:0;appearance:none;background:0 0;color:var(--text);font-size:13px;line-height:1.15;font-weight:780;cursor:pointer}.home-profile-picker select option{background:#171a20;color:var(--text)}.home-profile-picker:has(select:disabled){opacity:.58;cursor:not-allowed}.home-profile-picker select:disabled{cursor:not-allowed}.shot-chart-heading{min-width:0}.chart-head-actions{display:flex;align-items:center;justify-content:flex-end;flex-wrap:wrap}.chart-profile-picker-under{min-height:39px;margin:9px 0 0;padding:6px 11px 6px 13px;border-color:#4b5665;border-radius:12px}.chart-profile-picker-under>span{color:#9fa8b4;font-size:11px;letter-spacing:.04em}.chart-profile-picker-under select{min-width:132px;max-width:205px;font-size:14px;font-weight:820}.chart-profile-picker-under::before{display:none}.chart-expand-button,.chart-tool-button{line-height:1}.chart-tool-button.active{border-color:#9b673e;background:#3a2b21;color:#ffd3aa}.shot-profile-summary{display:grid;grid-template-columns:repeat(5,minmax(115px,1fr))}.shot-profile-chip{min-width:0;border:1px solid #3c4552;background:linear-gradient(145deg,#171c23,#11151a);color:var(--text);text-align:left;box-shadow:none}.shot-profile-chip:hover{border-color:#667385;background:#1b2129}.shot-profile-chip small{display:block;color:#9ca6b2;line-height:1.2;font-weight:720}.shot-profile-chip b{display:block;line-height:1.08;white-space:nowrap}.shot-scenario-inline{display:grid;grid-template-columns:repeat(4,minmax(0,1fr))}.shot-scenario-inline>span{min-width:0;border:1px solid #313844;background:#151920}.shot-scenario-inline small{font-weight:700}.home-quick-editor-copy b,.shot-scenario-inline b{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.shot-scenario-inline .pre{border-color:#315943}.shot-scenario-inline .pause{border-color:#4c405f}.shot-scenario-inline .main{border-color:#56492a}.shot-scenario-inline .total{border-color:#394b62}.shot-scenario-below{padding-top:1px}.home-quick-editor{display:grid;grid-template-columns:minmax(100px,1fr) minmax(150px,220px) auto auto;align-items:center;gap:9px;margin-top:8px;padding:11px 12px;border:1px solid #4a5665;border-radius:11px;background:#151a21}.home-quick-editor[hidden],.main-curve-editor[hidden]{display:none}.home-quick-editor-copy{min-width:0}.home-quick-editor-copy small,.main-curve-editor-copy span,.shot-scenario-inline small{display:block;color:var(--muted);line-height:1.15}.home-quick-editor-copy b{margin-top:3px;color:#e5e9ef;line-height:1.15}.home-quick-stepper{width:100%}.home-quick-stepper button,.home-quick-stepper input{min-height:38px;font-size:14px}.home-quick-editor-unit{min-width:22px;color:var(--muted);font-weight:700;text-align:center}.home-quick-editor-actions{display:flex;gap:6px}.home-quick-editor-actions button{width:38px;min-width:38px;height:38px;padding:0;font-size:16px}.main-curve-editor{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:8px 9px;border:1px solid #44505e;border-radius:11px;background:#151a21}.main-curve-editor-copy{min-width:0}.main-curve-editor-copy b{display:block;font-size:11px}.main-curve-editor-copy span{margin-top:2px;font-size:9px;line-height:1.25}.main-curve-actions{display:flex;flex:0 0 auto;gap:6px}.main-curve-actions button{min-width:0;padding:7px 10px;white-space:nowrap}.shot-chart-card.curve-editing #chart{touch-action:none}body.chart-expanded{overflow:hidden}.shot-chart-card.expanded{position:fixed;inset:12px;z-index:120;overflow:auto;border-color:#4d5968;box-shadow:0 0 0 100vmax rgba(0,0,0,.78),0 24px 80px rgba(0,0,0,.65)}.shot-chart-card.expanded #chart{height:calc(100vh - 245px);min-height:420px;max-height:none}.shot-chart-card.expanded .chart-tooltip{z-index:125}body.chart-expanded .toast{z-index:140}.temperature-chart-head{align-items:flex-start}.temperature-window-switch{flex:0 0 auto}.temperature-window-switch button{min-width:52px}.machine-detail,.machine-quick-actions .quick-action small{color:#89929f;font-size:11px;line-height:1.2;font-weight:600;letter-spacing:.01em}.machine-detail{min-height:14px}.home-temperature-panel{min-width:0}.home-temperature-panel .temperature-chart-head{display:flex}.home-temperature-panel .temperature-window-switch{flex:0 0 auto;gap:4px}.home-temperature-panel .temperature-window-switch button{min-width:45px;padding:6px 8px;font-size:10px}.home-temperature-panel #tempChart{width:100%}.home-temperature-panel .temperature-legend-bottom{justify-content:flex-start;gap:8px 11px}.machine-quick-actions{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));margin-top:15px;gap:9px}.machine-quick-actions .quick-action{grid-template-columns:34px minmax(0,1fr);justify-items:start;align-items:center;align-content:center;gap:9px;min-height:68px;padding:10px 11px;text-align:left;border-radius:12px}.machine-quick-actions .quick-icon{width:32px;height:32px;font-size:17px}.machine-quick-actions .quick-action b{font-size:13px;line-height:1.1}.machine-quick-actions .quick-action small{max-width:100%}.quick-action>span:last-child{min-width:0}.quick-action b,.quick-action small{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.quick-action small{margin-top:2px;color:var(--muted);font-weight:500}.quick-icon{display:grid;place-items:center;width:38px;height:38px;border-radius:50%;background:#303741;font-size:20px}.quick-action.active{background:#1b382a;border-color:#2d6949}.quick-action.active .quick-icon{background:#24533a;color:#9bf0bd}.quick-action.danger{background:#432429;border-color:#71343c}.quick-action.danger .quick-icon{background:#5a2930;color:#ffb6b6}.quick-action.water.active{background:#173248;border-color:#285d7e}.quick-action.water.active .quick-icon{background:#214b68;color:#8ed2ff}.quick-action.steam.active{background:#3a2b1b;border-color:#6c4b28}.quick-action.steam.active .quick-icon{background:#533b22;color:#ffc987}.machine-quick-actions .quick-action{position:relative;background:linear-gradient(145deg,#292f38,#20252d);border-color:#3d4652;color:#edf0f4;box-shadow:inset 0 1px 0 rgba(255,255,255,.045),0 5px 12px rgba(0,0,0,.18);transition:background .18s ease,border-color .18s ease,box-shadow .18s ease,color .18s ease,transform .12s ease}.machine-quick-actions .quick-action:not(.active):not(.danger):not(:disabled) .quick-icon{background:#343c47;color:#cbd2dc}.machine-quick-actions .quick-action:hover:not(:disabled):not(.active):not(.danger){background:linear-gradient(145deg,#313843,#262c35);border-color:#566171;box-shadow:inset 0 1px 0 rgba(255,255,255,.065),0 7px 16px rgba(0,0,0,.24);transform:translateY(-1px)}.machine-quick-actions .quick-action.active,.machine-quick-actions .quick-action.danger{box-shadow:inset 0 1px 0 rgba(255,255,255,.065),0 7px 18px rgba(0,0,0,.25)}.machine-quick-actions .quick-action.active small{color:rgba(255,255,255,.72)}.machine-quick-actions .quick-action.danger small{color:#ffb6b6}.machine-quick-actions .quick-action:disabled{opacity:1;background:#181c22;border-color:#292f38;color:#68717d;box-shadow:inset 0 1px 0 rgba(255,255,255,.018),inset 0 0 0 1px rgba(0,0,0,.1);cursor:not-allowed}.machine-quick-actions .quick-action:disabled .quick-icon{background:#22272e;color:#59626e}.machine-quick-actions .quick-action:disabled small{color:#59626e}.section-head{display:flex;align-items:flex-start;gap:12px;justify-content:space-between;margin-bottom:7px}.section-title{font-weight:750;font-size:16px}.section-subtitle{color:var(--muted);font-size:13px;line-height:1.3;margin-top:3px}.chart-switch,.legend{display:flex;flex-wrap:wrap}.chart-switch{gap:6px;justify-content:flex-end}.chart-switch button{background:#242a34;color:var(--muted);font-size:12px;line-height:1.2}.chart-switch button.active{background:#343b47;color:var(--text);box-shadow:inset 0 0 0 1px #4b5564}.legend{gap:13px;color:var(--muted);font-size:13px;line-height:1.35}.legend i{display:inline-block;width:9px;height:9px;border-radius:50%;margin-right:5px}.chart-legend-bottom{justify-content:center;margin-top:10px;padding-top:9px;border-top:1px solid #2b313a}.shot-legend-split{justify-content:space-between;gap:18px}.legend-group{display:flex;align-items:center;gap:13px;flex-wrap:wrap}.legend-group-right{justify-content:flex-end}.shot-legend-toggle{display:inline-flex;align-items:center;min-width:0;min-height:0;padding:2px 3px;border:0;border-radius:6px;background:0 0;color:inherit;font:inherit;line-height:inherit;box-shadow:none;cursor:pointer}.shot-legend-toggle:hover{background:rgba(255,255,255,.045);color:#d6dde5}.shot-legend-toggle.line-hidden{opacity:.42}.shot-legend-toggle.line-hidden i{opacity:.28!important}.shot-legend-toggle:focus-visible{outline:2px solid #566577;outline-offset:2px}canvas{width:100%;height:250px;display:block;margin-top:8px}.chart-wrap{position:relative;min-width:0}.chart-wrap canvas{touch-action:pan-y}.chart-tooltip{position:absolute;z-index:8;min-width:154px;padding:8px 10px;border:1px solid #3a424f;border-radius:8px;background:rgba(30,34,42,.96);box-shadow:0 10px 28px #0008;color:var(--text);font-size:13px;pointer-events:none;white-space:nowrap}.chart-tooltip[hidden]{display:none}.chart-tooltip-head,.chart-tooltip-row{display:grid;align-items:center;gap:7px}.chart-tooltip-row{grid-template-columns:16px 1fr auto}.chart-tooltip-head{grid-template-columns:1fr auto;padding-bottom:6px;margin-bottom:5px;border-bottom:1px solid #363c46;color:var(--muted)}.chart-tooltip-head b{color:var(--text);font-weight:700}.chart-tooltip-row{min-height:21px}.chart-tooltip-row span{color:var(--muted)}.chart-tooltip-row b{font-weight:700}.chart-tooltip-mark{width:13px;height:3px;border-radius:2px;background:currentColor}.chart-tooltip-mark.dashed{height:0;border-radius:0;border-top:3px dashed currentColor;background:0 0}.setting{display:grid;grid-template-columns:minmax(0,1fr) 115px;align-items:center;gap:12px;padding:12px 0;border-bottom:1px solid var(--line)}.setting:last-child,.status-line:last-child,.toggle-setting:last-child{border-bottom:0}input,select{width:100%;border:1px solid #343b47;background:#101318;color:var(--text);border-radius:11px;padding:10px}.diag{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:9px}.compact-metrics .metric b,.diag .metric b{font-size:18px}.footer-links{display:flex;gap:9px;flex-wrap:wrap;margin-top:12px}.notice{padding:12px 14px;border:1px solid #35404e;border-radius:13px;background:#151a21;color:var(--muted)}.control-note{margin-top:10px;color:var(--muted);font-size:12px}.card-actions{display:flex;gap:9px;flex-wrap:wrap;margin-top:14px}.card-actions>*{flex:1 1 150px}button.secondary{background:#29303a;color:#dce2ea}button.warning{background:#4a3921;color:#ffd79a}button.danger{background:#4b2528;color:#ffb6b6}.toggle-setting{display:flex;align-items:center;justify-content:space-between;gap:14px;padding:12px 0;border-bottom:1px solid var(--line)}.toggle-copy{min-width:0}.toggle-copy span{display:block;font-weight:650}.toggle-copy small{display:block;margin-top:2px;color:var(--muted)}.toggle-control{position:relative;width:46px;min-width:46px;height:26px}.toggle-control input{position:absolute;opacity:0;width:1px;height:1px;padding:0}.toggle-control i{position:absolute;inset:0;border-radius:999px;background:#343b47;box-shadow:inset 0 0 0 1px #47505e;transition:.18s}.toggle-control i::after{content:"";position:absolute;width:20px;height:20px;left:3px;top:3px;border-radius:50%;background:#d7dde6;transition:.18s}.toggle-control input:checked+i{background:#286746;box-shadow:inset 0 0 0 1px #3e9465}.toggle-control input:checked+i::after{transform:translateX(20px);background:#effff5}details.advanced{margin-top:12px;border-top:1px solid var(--line);padding-top:10px}details.advanced summary{cursor:pointer;color:var(--muted);font-weight:650}details.advanced[open] summary{color:var(--text);margin-bottom:5px}.status-line{display:flex;align-items:center;justify-content:space-between;gap:12px;padding:10px 0;border-bottom:1px solid var(--line)}.status-line span{color:var(--muted)}.status-value{text-align:right;font-weight:700;overflow-wrap:anywhere}.compact-metrics{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:9px;margin-top:10px}.recipe-workspace{display:grid;grid-template-columns:minmax(390px,.88fr) minmax(480px,1.12fr);gap:12px;align-items:start}.recipe-editor-card,.recipe-editor-column{min-width:0}.recipe-editor-head,.recipe-preview-head{display:flex;align-items:flex-start;justify-content:space-between;gap:12px}.recipe-preview-status,.recipe-profile-badge{flex:0 0 auto;padding:6px 9px;border:1px solid #3a424f;border-radius:999px;background:#20252d;color:var(--muted);font-size:11px;font-weight:750}.recipe-preview-status.dirty,.recipe-profile-badge.dirty{color:#ffd98a;border-color:#6f5830;background:#382f1f}.recipe-profile-select{margin-top:12px}.recipe-description{margin-top:10px;padding:11px 12px;border:1px solid #303744;border-radius:12px;background:#11151b;color:#cfd5de;font-size:12px;line-height:1.5}.recipe-group-title{margin:15px 0 8px;color:var(--muted);font-size:11px;font-weight:800;letter-spacing:.055em;text-transform:uppercase}.recipe-controls-grid,.recipe-drink-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(250px,1fr));gap:8px}.recipe-control{min-width:0;display:grid;grid-template-columns:minmax(90px,1fr) minmax(120px,134px);align-items:center;gap:9px;padding:9px 10px;border:1px solid var(--line);border-radius:12px;background:#15191f}.recipe-control-copy{min-width:0}.recipe-control-copy span{display:block;font-size:12px;font-weight:700;line-height:1.2;overflow-wrap:anywhere}.recipe-control-copy small{display:block;margin-top:2px;color:var(--muted);font-size:10px}.number-stepper{min-width:0;display:grid;grid-template-columns:32px minmax(50px,1fr) 32px;align-items:center;overflow:hidden;border:1px solid #39414d;border-radius:10px;background:#20252c}.number-stepper button,.number-stepper input{min-width:0;height:34px;border:0;border-radius:0;background:0 0}.number-stepper button{padding:0;color:#d8dee7;font-size:18px}.number-stepper button:hover{background:#2c323c}.number-stepper input{width:100%;padding:0 4px;border-left:1px solid #39414d;border-right:1px solid #39414d;text-align:center;font-weight:800;-moz-appearance:textfield}.number-stepper input::-webkit-inner-spin-button,.number-stepper input::-webkit-outer-spin-button{-webkit-appearance:none;margin:0}.recipe-pressure-grid{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px}.recipe-pressure-grid>label{min-width:0;padding:9px;border:1px solid var(--line);border-radius:12px;background:#15191f}.recipe-pressure-grid>label>span{display:block;margin-bottom:7px;font-size:11px;font-weight:700}.recipe-pressure-grid>label>small{margin-top:5px;text-align:center}.number-stepper.compact{grid-template-columns:29px minmax(38px,1fr) 29px}.number-stepper.compact button,.number-stepper.compact input{height:31px}.recipe-weight-toggle{margin-top:9px;padding:10px 2px 2px}.recipe-editor-actions{display:grid;grid-template-columns:1fr 1.15fr;gap:9px;margin-top:14px}.recipe-editor-actions button{min-height:42px}.recipe-preview-card{position:sticky;top:10px;min-width:0}.recipe-preview-chips{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px;margin-top:12px}.recipe-preview-chips>span{min-width:0;padding:9px 10px;border:1px solid var(--line);border-radius:11px;background:#14181e}.recipe-phase-strip small,.recipe-pressure-grid>label>small,.recipe-preview-chips small{display:block;color:var(--muted);font-size:9px}.recipe-phase-strip b,.recipe-preview-chips b{display:block;margin-top:3px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;font-size:14px}.recipe-legend{margin-top:12px}#recipeChart{height:355px;margin-top:7px;touch-action:none;cursor:default}.recipe-drag-hint{margin-top:7px;color:var(--muted);font-size:10px;line-height:1.35}.recipe-phase-strip{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:7px;margin-top:8px}.recipe-phase-strip>span{min-width:0;padding:8px 9px;border:1px solid var(--line);border-radius:10px;background:#15191f}.recipe-phase-strip b{font-size:12px}.recipe-soft-summary{margin-top:8px;padding:9px 10px;border-left:3px solid #54a9ff;border-radius:8px;background:rgba(84,169,255,.08);color:#bfc9d7;font-size:11px}@media (max-width:1080px){.recipe-workspace{grid-template-columns:minmax(360px,.95fr) minmax(420px,1.05fr)}.recipe-controls-grid{grid-template-columns:1fr}}@media (max-width:840px){.recipe-workspace{grid-template-columns:1fr}.recipe-preview-card{position:static}.recipe-controls-grid{grid-template-columns:repeat(2,minmax(0,1fr))}}@media (max-width:620px){.recipe-controls-grid,.recipe-drink-grid,.recipe-pressure-grid{grid-template-columns:1fr}.recipe-pressure-grid>label{display:grid;grid-template-columns:minmax(70px,1fr) minmax(130px,1.5fr) 28px;align-items:center;gap:7px}.recipe-pressure-grid>label>small,.recipe-pressure-grid>label>span{margin:0;text-align:left}.recipe-preview-chips{grid-template-columns:1fr 1fr}.recipe-preview-chips>span:first-child{grid-column:1/-1}#recipeChart{height:290px}}@media (max-width:430px){.recipe-control{grid-template-columns:1fr}.recipe-preview-status,.recipe-profile-badge{max-width:120px;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.recipe-phase-strip{grid-template-columns:1fr}}.toast{position:fixed;right:16px;bottom:16px;z-index:50;background:#232832;border:1px solid #3a424f;padding:11px 14px;border-radius:12px;opacity:0;transform:translateY(10px);transition:.2s;pointer-events:none}.toast.show{opacity:1;transform:none}@media (max-width:840px){.shell{display:block;padding:10px}.home-top-grid{grid-template-columns:minmax(350px,3fr) minmax(290px,2fr)}.home-layout-toolbar{align-items:flex-start}.home-layout-hint{max-width:55%}.machine-row{min-height:150px}.content{max-width:none;transform:none}.sidebar{position:fixed;top:0;left:0;width:min(82vw,270px);height:100vh;min-height:0;border-radius:0 18px 18px 0;transform:translateX(-105%);transition:transform .2s ease}body.nav-open .sidebar{transform:translateX(0)}body.sidebar-collapsed .nav-label,body.sidebar-collapsed .side-brand,body.sidebar-collapsed .side-footer{opacity:1;width:auto;pointer-events:auto}body.sidebar-collapsed .nav-label{display:block}body.sidebar-collapsed .nav-item{justify-content:flex-start;padding:11px 12px}.backdrop{position:fixed;inset:0;z-index:15;background:#0009}body.nav-open .backdrop{display:block}.mobile-menu{display:inline-grid;place-items:center;width:42px;height:42px;padding:0}header{margin-bottom:10px}.brand{font-size:18px}}@media (max-width:700px){.grid,.hero{grid-template-columns:1fr}.home-dashboard-layout{grid-template-columns:minmax(0,1fr)}.dashboard-block-wide{grid-column:1}.home-top-grid{grid-template-columns:minmax(0,1fr)}.home-temperature-panel #tempChart{height:205px}.home-temperature-panel .temperature-legend-bottom{font-size:11px}.machine-row{min-height:138px}.power-inline{min-width:86px;padding:7px 9px}.machine-quick-actions .quick-action,.quick-action{grid-template-columns:1fr;justify-items:center;text-align:center}.quick-action{gap:6px;padding:9px 6px}.machine-quick-actions{margin-top:11px;gap:6px}.machine-quick-actions .quick-action{min-height:61px;padding:7px 4px}.pressure strong{font-size:46px}.diag{grid-template-columns:repeat(2,minmax(0,1fr))}.card{border-radius:16px;padding:14px}.section-head{display:block}.chart-head-actions,.chart-switch{justify-content:flex-start;margin-top:10px}.chart-switch{margin-top:0}.shot-profile-summary{grid-template-columns:repeat(3,minmax(0,1fr))}.shot-scenario-inline{grid-template-columns:repeat(2,minmax(0,1fr))}.home-quick-editor{grid-template-columns:minmax(90px,1fr) minmax(145px,200px) auto auto}.main-curve-editor{align-items:flex-start}.temperature-chart-head{display:flex;align-items:flex-start}.temperature-window-switch{margin-top:0}.shot-chart-card.expanded{inset:6px;padding:13px}.shot-chart-card.expanded #chart{height:calc(100vh - 330px);min-height:330px}canvas{height:235px}.shot-chart-card #chart{height:290px}}@media (max-width:430px){.home-profile-picker>span,.online,.power-inline #powerButtonLabel{display:none}.home-layout-toolbar{display:block}.home-layout-hint{max-width:none;margin-bottom:7px}.home-layout-actions{justify-content:flex-end}.home-hidden-blocks,.shot-legend-split{align-items:flex-start}.legend-group{gap:9px}.machine-row{min-height:132px}.power-inline{min-width:42px;padding:7px 8px;border-radius:10px}.power-inline .power-symbol{font-size:17px}.machine-card{min-height:100%}.home-profile-picker{margin-left:0;padding-left:9px}.home-profile-picker select{min-width:104px;max-width:145px}.chart-head-actions{gap:7px}.chart-profile-picker{flex:0 1 auto;min-width:0}.chart-profile-picker select{min-width:96px;max-width:128px}.chart-switch{flex:1 1 auto}.chart-switch button{flex:1 1 auto;padding-left:8px;padding-right:8px}.chart-tool-button{flex:0 0 36px}.shot-profile-summary{grid-template-columns:repeat(2,minmax(0,1fr))}.shot-scenario-inline{grid-template-columns:minmax(0,1fr)}.home-quick-editor{grid-template-columns:minmax(0,1fr) auto auto}.home-quick-editor-copy{grid-column:1/-1}.home-quick-stepper{min-width:0}.main-curve-editor{display:block}.main-curve-actions{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));margin-top:7px}.main-curve-actions button{width:100%}.temperature-chart-head{display:block}.home-temperature-panel #tempChart{height:190px}.home-temperature-panel .temperature-window-switch{margin-top:8px}.temperature-window-switch{margin-top:9px}.machine-auto-off{padding:4px 7px}.machine-auto-off b{font-size:12px}.shot-chart-card.expanded #chart{height:calc(100vh - 430px);min-height:280px}.quick-action small{font-size:10px}.machine-quick-actions .quick-action small{display:block;font-size:10px}.machine-quick-actions .quick-action{min-height:62px}.machine-quick-actions .quick-icon{width:26px;height:26px;font-size:14px}.setting{grid-template-columns:1fr;gap:7px}.diag{grid-template-columns:1fr 1fr}.legend{gap:8px 11px}}.home-top-grid{grid-template-columns:repeat(2,minmax(0,1fr))}.quick-action:disabled{opacity:1;cursor:not-allowed;border-color:#39424e;background:linear-gradient(145deg,#1b2027,#15191f);color:#b9c1cb;box-shadow:inset 0 1px 0 rgba(255,255,255,.025)}.quick-action:disabled .quick-icon{background:#29313a;color:#8f99a6}.quick-action:disabled b{color:#c5ccd5}.quick-action:disabled small{color:#858f9c}.temperature-mode-switch{display:flex;gap:4px;flex:0 0 auto}.home-temperature-state{display:flex;flex-direction:column;justify-content:center;border:1px solid #303844;border-radius:14px;background:linear-gradient(145deg,#171c23,#11151a)}.home-temperature-graph[hidden],.home-temperature-state[hidden]{display:none}.temperature-state-main{display:flex;justify-content:space-between;gap:14px}.profile-rail-head small,.temperature-state-pid small,.temperature-state-value small{display:block;color:#939daa;font-weight:720}.temperature-state-value strong{display:block;color:#edf1f6;line-height:.98;font-weight:820;white-space:nowrap}.home-metric-copy span i,.home-metric-copy strong i,.temperature-state-pid strong i,.temperature-state-value span i,.temperature-state-value strong i{font-style:normal}.temperature-state-value strong em{color:#aab3bf;font-style:normal;font-weight:700}.temperature-state-value>span{display:block;color:#9ba5b1}.temperature-state-icon{display:grid;place-items:center;background:#242b34;color:#e4a06d}.temperature-state-icon svg{fill:none;stroke:currentColor;stroke-width:1.7;stroke-linecap:round;stroke-linejoin:round}.temperature-state-pid{display:flex;justify-content:space-between;gap:12px}.temperature-state-pid strong{display:block;line-height:1}.temperature-state-pid strong em{color:#9ca6b2;font-size:14px;font-style:normal}.pid-state-badge{display:inline-flex;align-items:center;border:1px solid #404955;border-radius:999px;background:#20252c;color:#aab3bf;font-weight:800}.home-temperature-state.heating .pid-state-badge{border-color:#70512f;background:#2b2118;color:#ffc27f}.home-temperature-state.ready .pid-state-badge{border-color:#35664d;background:#182a21;color:#8ee9b3}.home-temperature-state.error .pid-state-badge{border-color:#704047;background:#2d2023;color:#ff9d9d}.pid-output-track{height:6px;overflow:hidden;border-radius:999px;background:#252c35}.home-metric-progress i,.pid-output-track i{display:block;width:0;height:100%;border-radius:inherit;background:linear-gradient(90deg,#b66a38,#f0a25f,#ffd166);transition:width .25s ease}.temperature-state-delta{color:#939daa;text-align:right}.temperature-graph-toolbar{display:flex;align-items:center;justify-content:space-between;gap:10px;color:#929ca9;font-size:10px;font-weight:700}.home-temperature-panel #tempChart{height:132px;margin-top:2px}.shot-chart-workspace{display:grid}.shot-profile-rail{position:sticky;top:12px;min-width:0;border:1px solid #38424f;border-radius:14px;background:linear-gradient(145deg,#181d24,#12161b)}.profile-rail-head small{color:#929ca8;font-size:10px}.profile-rail-head b{display:block;margin-top:4px;overflow:hidden;color:#eef2f6;line-height:1.15;text-overflow:ellipsis;white-space:nowrap}.profile-rail-select{display:none;width:100%;margin:11px 0 0;border-radius:11px}.profile-rail-select select{width:100%;min-width:0;max-width:none}.home-profile-buttons{display:grid}.home-profile-buttons button{border:1px solid #343e4a;border-radius:10px;background:#1b2027;color:#adb6c1;text-align:left;font-weight:700}.home-profile-buttons button:hover{border-color:#566272;background:#222831}.home-profile-buttons button.active{border-color:#9b673e;background:#35291f;color:#ffd1aa}.home-profile-buttons button:disabled{opacity:.58;cursor:not-allowed}.profile-recipes-button{width:100%;border-radius:10px}.home-quick-editor-copy small{font-size:12px;font-weight:720}.home-quick-editor-copy b{font-size:16px}.home-quick-editor-unit{font-size:13px}@media (max-width:980px){.shot-chart-workspace{grid-template-columns:minmax(0,1fr)}.shot-profile-rail{position:static}.home-profile-buttons{grid-template-columns:repeat(3,minmax(0,1fr))}.home-profile-buttons button{text-align:center}}@media (max-width:760px){.home-top-grid{grid-template-columns:minmax(0,1fr)}.home-temperature-state{min-height:205px}}@media (max-width:560px){.temperature-mode-switch button{min-width:0;padding:7px 8px}.temperature-state-value strong{font-size:36px}.home-profile-buttons{display:none}.profile-rail-select{display:flex}.shot-profile-rail{padding:11px}}.machine-panel{display:grid;grid-template-rows:auto auto;align-content:stretch;gap:10px}.machine-row{min-width:0;height:auto}.machine-card-restored{min-height:0;height:auto;padding:15px 17px;justify-content:flex-start}.machine-card-head{align-items:center}.machine-identity{min-width:0}.machine-status-row{display:flex;align-items:center;flex-wrap:wrap;gap:7px 9px;margin-top:4px}.machine-status-row .machine-state{margin-top:0;font-size:13px}.machine-water-status{display:inline-flex;align-items:center;gap:5px;min-height:25px;padding:4px 8px;border:1px solid #3b4551;border-radius:999px;background:#20252c;color:#98a2af;line-height:1;white-space:nowrap}.machine-water-status>span{display:grid;place-items:center;width:15px;height:15px;border-radius:50%;background:#303741;color:#a8b1bd;font-size:9px;font-weight:900}.machine-water-status b{font-size:10px;font-weight:800}.machine-water-status.ok{border-color:#35664d;background:#182a21;color:#91eab5}.machine-water-status.ok>span{background:#23543a;color:#b3f4cc}.machine-water-status.warn{border-color:#704047;background:#302125;color:#ff9da4}.machine-water-status.warn>span{background:#5b2931;color:#ffd0d3}.machine-reading{margin-top:14px}.machine-temperature b{font-size:28px;line-height:1}.machine-detail{margin-top:7px}.machine-actions-card{min-width:0;padding:10px}.machine-actions-card .machine-quick-actions{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:9px;margin:0}.machine-actions-card .quick-action{display:grid;grid-template-columns:minmax(0,1fr);justify-items:center;align-content:center;gap:7px;min-height:88px;padding:10px 8px;border-radius:13px;color:#edf1f5;text-align:center}.machine-actions-card .quick-action>span:last-child{width:100%;min-width:0}.machine-control-card .quick-action>span:last-child{min-width:0}.machine-actions-card .quick-icon{display:grid;place-items:center;width:38px;height:38px;margin:0 auto;border-radius:12px;background:#3a434e;color:#d6dde6;font-size:19px}.machine-actions-card .quick-action b{color:#eef2f6;font-size:13px;line-height:1.1}.machine-actions-card .quick-action small{display:block;margin-top:4px;color:#a1abb8;font-size:10px;line-height:1.15}.machine-control-card .quick-action small{color:#a1abb8}.machine-actions-card .quick-action:hover:not(:disabled):not(.active):not(.danger){border:0;background:linear-gradient(145deg,#353d48,#29313a)}.machine-actions-card .quick-action:disabled{opacity:1;color:#cbd2db;cursor:not-allowed;border:0;background:linear-gradient(145deg,#252c35,#1e242b);box-shadow:inset 0 1px 0 rgba(255,255,255,.03),0 3px 9px rgba(0,0,0,.14)}.machine-actions-card .quick-action:disabled .quick-icon{background:#343d48;color:#aeb8c4}.machine-actions-card .quick-action:disabled b,.machine-control-card .quick-action:disabled b{color:#d5dbe3}.machine-actions-card .quick-action:disabled small,.machine-control-card .quick-action:disabled small{color:#8f9aa7}.shot-chart-head .section-title{font-weight:750}.home-temperature-panel .section-title{line-height:1.15;letter-spacing:0;font-weight:750}@media (max-width:700px){.machine-row{min-height:0}.machine-actions-card .quick-action{min-height:78px}}@media (max-width:430px){.machine-card-restored{padding:14px}.machine-actions-card{padding:8px}.machine-actions-card .machine-quick-actions{gap:6px}.machine-actions-card .quick-action{min-height:72px;padding:8px 4px}.machine-actions-card .quick-icon{width:31px;height:31px;font-size:16px}.machine-actions-card .quick-action b{font-size:12px}.machine-water-status b{font-size:9px}}.machine-actions-card .quick-action{border:0;background:linear-gradient(145deg,#2c333d,#222830);box-shadow:inset 0 1px 0 rgba(255,255,255,.05),0 5px 13px rgba(0,0,0,.18)}.machine-actions-card .quick-action.active,.machine-actions-card .quick-action.danger,.machine-actions-card .quick-action.steam.active,.machine-actions-card .quick-action.water.active{border:0}.machine-panel{display:block}.machine-control-card{display:flex;flex-direction:column;justify-content:flex-start}.machine-control-head{display:flex;align-items:center;justify-content:space-between}.machine-control-state{min-width:0}.machine-control-state>small{display:block;color:#929ca9;font-size:11px;line-height:1.1;font-weight:720;margin-top:5px}.machine-control-state .machine-state{display:block;color:#e8edf3;font-size:22px;line-height:1.05;font-weight:820}.machine-control-state .machine-state.ready{color:var(--green)}.machine-control-state .machine-state.busy{color:var(--accent)}.machine-control-state .machine-state.error{color:var(--red)}.machine-status-strip{display:flex;align-items:center;flex-wrap:wrap;gap:8px;margin-top:16px}.machine-mode-status>span:first-child{color:#848f9c;font-size:9px;line-height:1;font-weight:750;text-transform:uppercase;letter-spacing:.035em}.machine-mode-status b{color:#cfd6df;font-size:10px;line-height:1;font-weight:750}.machine-control-divider{height:1px;background:linear-gradient(90deg,transparent,#343d48 12%,#343d48 88%,transparent)}.machine-control-card .machine-quick-actions{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));margin:0}.machine-control-card .quick-action{display:grid;border:0;background:linear-gradient(145deg,#2c333d,#222830);color:#edf1f5;box-shadow:inset 0 1px 0 rgba(255,255,255,.05),0 5px 13px rgba(0,0,0,.18)}.machine-control-card .quick-icon{display:grid;place-items:center;background:#3a434e;color:#d6dde6}.machine-control-card .quick-action b{color:#eef2f6}.machine-control-card .quick-action:hover:not(:disabled):not(.active):not(.danger){border:0;background:linear-gradient(145deg,#353d48,#29313a)}.machine-control-card .quick-action:disabled{opacity:1;border:0;background:linear-gradient(145deg,#252c35,#1e242b);color:#cbd2db;box-shadow:inset 0 1px 0 rgba(255,255,255,.03),0 3px 9px rgba(0,0,0,.14);cursor:not-allowed}.machine-control-card .quick-action:disabled .quick-icon{background:#343d48;color:#aeb8c4}.machine-control-card .quick-action.active,.machine-control-card .quick-action.danger,.machine-control-card .quick-action.steam.active,.machine-control-card .quick-action.water.active{border:0}@media (max-width:700px){.machine-control-card{min-height:0;height:auto}.machine-control-card .quick-action{min-height:82px}}@media (max-width:430px){.machine-control-card{padding:14px}.machine-control-card .power-inline{min-width:94px;padding:7px 9px}.machine-control-card .machine-quick-actions,.machine-status-strip{gap:6px}.machine-control-card .quick-action{min-height:74px;padding:8px 4px}.machine-control-card .quick-icon{width:31px;height:31px;font-size:16px}.machine-control-card .quick-action b{font-size:12px}}.home-layout-toolbar{display:none!important}.home-layout-header-button{display:grid;place-items:center;padding:0;border:1px solid #343e4a;background:#1c2229;color:#a8b2bf;box-shadow:none}.home-layout-header-button[hidden]{display:none}.home-layout-header-button svg{fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round;width:17px;height:17px}.home-layout-header-button:hover{border-color:#566272;background:#252c35;color:#d7dde5}.home-layout-header-button.active{border-color:#9b673e;background:#35291f;color:#ffd1aa}.home-layout-tools{display:flex;align-items:center;flex-wrap:wrap;gap:8px;margin:0 0 10px;padding:8px 10px;border:1px dashed #495563;border-radius:11px;background:#151a20}.brand small,.home-layout-tools[hidden]{display:none}.home-layout-tools .home-layout-hint{flex:1 1 260px;margin:0}.home-layout-tools .home-layout-actions{flex:0 0 auto}.home-layout-tools .home-hidden-blocks{flex:1 0 100%;margin:0;padding:7px 8px}.machine-control-state{display:grid;grid-template-rows:auto auto auto;align-content:start}.machine-control-title{color:#eef2f6;font-weight:820}.machine-control-state .machine-state{margin-top:4px}.machine-control-card .machine-quick-actions{align-items:stretch}.machine-control-card .quick-action{width:100%}.machine-control-card .quick-icon{align-self:start}.machine-control-card .quick-action>span:last-child{display:grid;grid-template-rows:16px 18px;align-content:start;align-items:start}.machine-control-card .quick-action b,.machine-control-card .quick-action small{display:block}@media (max-width:700px){.machine-control-card .machine-quick-actions{grid-auto-rows:82px}.machine-control-card .quick-action{height:82px;grid-template-rows:33px 34px}.machine-control-card .quick-action>span:last-child{min-height:34px}}@media (max-width:430px){.home-layout-header-button{width:32px;min-width:32px;height:32px}.home-layout-tools{align-items:flex-start}.machine-control-card .machine-quick-actions{grid-auto-rows:74px}.machine-control-card .quick-action{height:74px;grid-template-rows:31px 31px}.machine-control-card .quick-action>span:last-child{grid-template-rows:15px 15px;min-height:31px}.machine-control-card .quick-action b,.machine-control-card .quick-action small{line-height:15px}}header{min-height:42px;margin-bottom:7px;gap:9px}.brand{font-size:16px;line-height:1}.online{padding:5px 8px;font-size:10px}.home-layout-header-button{width:30px;min-width:30px;height:30px;border-radius:9px}.home-dashboard-layout{gap:9px}.home-top-grid{gap:9px;align-items:stretch}.home-overview{gap:9px}.machine-panel{height:100%}.machine-control-head{gap:10px}.machine-control-primary{display:flex;min-width:0;align-items:center;flex-wrap:wrap;gap:7px 9px}.machine-control-title{font-size:16px;line-height:1}.machine-control-primary .machine-state{display:inline-flex;min-height:24px;align-items:center;margin:0;padding:4px 8px;border:1px solid #3b4551;border-radius:999px;background:#1c2229;color:#a9b3bf;font-size:10px;line-height:1;font-weight:800}.machine-control-primary .machine-state.ready{border-color:#35664d;background:#182a21;color:#91eab5}.machine-control-primary .machine-state.busy{border-color:#6d502f;background:#2b2118;color:#ffc27f}.machine-control-primary .machine-state.error{border-color:#704047;background:#302125;color:#ff9da4}.machine-control-head-actions{display:flex;flex:0 0 auto;align-items:center;gap:7px}.machine-control-card .machine-water-status{min-height:28px;padding:4px 8px}.machine-control-card .power-inline{min-width:96px;min-height:34px;padding:6px 9px;border-radius:10px;font-size:10px}.machine-control-card .power-inline .power-symbol{font-size:16px}.machine-secondary-row{display:flex;min-height:28px;align-items:center;flex-wrap:wrap;gap:7px;margin-top:8px}.machine-control-card .machine-auto-off,.machine-mode-status{min-height:27px;padding:4px 8px}.machine-mode-status{display:inline-flex;align-items:center;gap:6px;border:1px solid #343e4a;border-radius:999px;background:#1b2027;white-space:nowrap}.machine-mode-dot{width:6px;height:6px;border-radius:50%;background:#6f7b89}.machine-control-divider{margin:9px 0 8px}.machine-control-card .machine-quick-actions{grid-auto-rows:66px;gap:7px}.machine-control-card .quick-action{height:66px;min-height:66px;grid-template-columns:29px minmax(0,1fr);grid-template-rows:1fr;align-items:center;justify-items:start;align-content:center;gap:8px;padding:7px 8px;border-radius:11px;text-align:left}.machine-control-card .quick-icon{width:29px;height:29px;margin:0;border-radius:9px;font-size:15px}.machine-control-card .quick-action>span:last-child{display:block;width:100%;min-height:0}.machine-control-card .quick-action b,.machine-control-card .quick-action small{width:100%;margin:0;text-align:left;line-height:1.1;font-size:12px}.machine-control-card .quick-action small{margin:3px 0 0;font-size:9px}.home-temperature-panel{min-height:0;height:100%;padding:11px 12px 10px}.home-temperature-panel .temperature-chart-head{align-items:center;margin-bottom:0}.home-temperature-panel .section-title{font-size:16px}.home-temperature-panel .section-subtitle{margin-top:2px;font-size:9px}.temperature-mode-switch button{min-width:62px;min-height:29px;padding:5px 8px;font-size:9px}.home-temperature-state{min-height:132px;display:grid;grid-template-columns:minmax(0,1.25fr) minmax(110px,.75fr);grid-template-rows:auto 5px auto;align-items:center;gap:7px 11px;margin-top:6px;padding:10px 11px}.temperature-state-main{min-width:0;align-items:center}.temperature-state-pid small,.temperature-state-value small{font-size:10px}.temperature-state-value strong{margin-top:3px;font-size:32px}.temperature-state-value strong em{font-size:14px}.temperature-state-value>span{margin-top:4px;font-size:10px}.temperature-state-icon{width:44px;height:44px;border-radius:13px}.temperature-state-icon svg{width:25px;height:25px}.temperature-state-pid{min-width:0;height:100%;align-items:center;padding:0 0 0 11px;border-top:0;border-left:1px solid #2f3742}.temperature-state-pid strong{margin-top:3px;font-size:22px}.pid-state-badge{min-height:25px;padding:4px 8px;font-size:9px}.home-temperature-state .pid-output-track{grid-column:1/-1;height:5px}.temperature-state-delta{grid-column:1/-1;font-size:9px}.home-temperature-graph{margin-top:5px}.temperature-graph-toolbar{margin-bottom:2px}.home-temperature-panel .temperature-legend-bottom{margin-top:3px;padding-top:5px;font-size:9px}@media (max-width:760px){.home-temperature-panel,.machine-control-card{height:auto}.home-temperature-state{min-height:138px}}@media (max-width:520px){header{min-height:38px;margin-bottom:6px}.brand{font-size:15px}.machine-control-head{align-items:flex-start}.machine-control-head-actions{gap:5px}.machine-control-card .machine-water-status b{display:none}.machine-control-card .machine-water-status{width:28px;padding:4px;justify-content:center}.machine-control-card .machine-quick-actions{grid-auto-rows:62px}.machine-control-card .quick-action{height:62px;min-height:62px;grid-template-columns:1fr;justify-items:center;gap:4px;padding:6px 4px;text-align:center}.machine-control-card .quick-icon{margin:0 auto}.machine-control-card .quick-action b,.machine-control-card .quick-action small{text-align:center}.machine-control-card .quick-action small{display:none}.home-temperature-state{grid-template-columns:minmax(0,1fr);grid-template-rows:auto auto 5px auto}.temperature-state-pid{padding:8px 0 0;border-left:0;border-top:1px solid #2f3742}}.home-metrics-panel{min-width:0;height:100%}.home-metrics-head{display:flex;align-items:flex-start;justify-content:space-between;gap:10px;margin-bottom:7px}.home-metrics-head .section-title{font-size:16px;line-height:1.15}.home-metrics-head .section-subtitle{margin-top:2px;font-size:9px}.home-metrics-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));grid-template-rows:repeat(2,minmax(0,1fr))}.home-metric-card{position:relative;min-width:0;display:grid;grid-template-columns:31px minmax(0,1fr);align-items:center;gap:8px;overflow:hidden;border:1px solid #303945;border-radius:12px;background:linear-gradient(145deg,#171c23,#11151a)}.home-metric-icon{display:grid;place-items:center;width:31px;height:31px;border-radius:10px;background:#242b34;color:#aeb8c4;font-size:17px;font-style:normal}.home-metric-icon svg{width:20px;height:20px;fill:none;stroke:currentColor;stroke-width:1.7;stroke-linecap:round;stroke-linejoin:round}.temperature-metric .home-metric-icon{color:#e6a16e}.pressure-metric .home-metric-icon{color:#78baff}.weight-metric .home-metric-icon{color:#7ce0ad}.cleaning-metric .home-metric-icon{color:#b59ae9}.home-metric-copy{min-width:0}.home-metric-copy small{display:block;color:#929ca9;line-height:1.1;font-weight:720}.home-metric-copy strong{display:block;margin-top:3px;color:#edf1f6;line-height:1;font-weight:820;font-size:23px}.home-metric-copy strong em{color:#9ca6b2;font-size:11px;font-style:normal;font-weight:700}.home-metric-copy>span{display:block;margin-top:5px;color:#8f99a6;line-height:1.15}.home-metric-progress{position:absolute;right:9px;bottom:7px;left:49px;height:3px;overflow:hidden;border-radius:999px;background:#262d36}.home-metric-copy strong,.home-metric-copy>span,.home-metric-state{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.home-metric-state{position:absolute;top:8px;right:8px;max-width:64px;color:#9ca6b2;font-size:7px;font-weight:800}.weight-metric.live{border-color:#35664d;background:linear-gradient(145deg,#18271f,#111a15);box-shadow:inset 0 0 0 1px rgba(75,209,139,.08)}.weight-metric.live .home-metric-copy strong{color:#9df0bf}.cleaning-metric.due{border-color:#70512f;background:linear-gradient(145deg,#2b2118,#18130f)}#page-home .cleaning-metric.due .home-backflush-main strong,.cleaning-metric.due .home-metric-copy strong,.cleaning-metric.due .home-metric-icon{color:#ffc27f}.shot-chart-card{padding:16px 17px 15px}.shot-chart-head{align-items:center}.shot-chart-head .section-title{font-size:16px;line-height:1.15;letter-spacing:0}.shot-chart-context{margin-top:3px;font-size:10px}.chart-head-actions{gap:6px}.chart-expand-button,.chart-tool-button{width:32px;min-width:32px;height:32px;padding:0;font-size:15px;border-radius:9px}.chart-expand-button{font-size:16px}.shot-profile-summary{gap:7px;margin-top:10px}.shot-profile-chip{min-height:54px;padding:8px 10px;border-radius:11px}.shot-profile-chip small,.shot-scenario-inline small{font-size:9px}.shot-profile-chip b{margin-top:4px;font-size:15px}.shot-chart-card #chart{height:300px;margin-top:5px}.shot-legend-split{margin-top:7px;padding-top:7px;font-size:11px}.shot-scenario-inline{gap:7px;margin-top:8px}.shot-scenario-inline>span{min-height:49px;padding:8px 9px;border-radius:9px}.shot-scenario-inline b{margin-top:4px;font-size:12px}.shot-profile-rail{padding:11px}.profile-rail-head b{font-size:16px}.home-profile-buttons{gap:5px;margin-top:9px}.home-profile-buttons button{min-height:33px;padding:6px 9px;font-size:11px}.profile-recipes-button{margin-top:8px;padding:7px 9px;font-size:10px}@media (max-width:700px){.home-metrics-grid{min-height:150px}.shot-chart-card #chart{height:270px}}@media (max-width:430px){.home-metrics-grid{grid-template-columns:minmax(0,1fr);grid-template-rows:repeat(4,minmax(62px,auto))}.home-metric-card{min-height:62px}.shot-profile-summary{grid-template-columns:repeat(2,minmax(0,1fr))}.shot-chart-card #chart{height:245px}}.home-metrics-panel{display:flex;flex-direction:column;justify-content:stretch;padding:10px}.home-metrics-grid{flex:1 1 auto;min-height:164px;gap:8px}.home-metric-card{padding:11px}.home-metric-copy small{font-size:10px}.home-metric-copy>span{font-size:9px}@media (max-width:700px){.home-metrics-grid{min-height:172px}}@media (max-width:430px){.home-metrics-grid{min-height:0}}.temperature-metric-open{cursor:pointer;outline:0;transition:border-color .18s ease,background .18s ease,box-shadow .18s ease,transform .12s ease}.temperature-metric-open:focus-visible,.temperature-metric-open:hover{border-color:#76563c;background:linear-gradient(145deg,#211b17,#151310);box-shadow:inset 0 0 0 1px rgba(225,139,72,.08)}.temperature-metric-open:active{transform:translateY(1px)}.temperature-metric-open.expanded{border-color:#8a603f;background:linear-gradient(145deg,#241c17,#17130f)}.metric-open-indicator{position:absolute;top:7px;right:8px;display:grid;place-items:center;width:19px;height:19px;border-radius:7px;background:#2a3038;color:#aab4c0;font-size:10px;line-height:1;transition:transform .18s ease,background .18s ease,color .18s ease}.temperature-metric-open.expanded .metric-open-indicator{transform:rotate(90deg);background:#4a3425;color:#ffd0a8}.temperature-metric-open .home-metric-state{right:31px;max-width:55px}.temperature-details-card{margin-top:9px;padding:16px 17px 13px}.temperature-details-card[hidden]{display:none}.temperature-details-head{align-items:center;margin-bottom:2px}.temperature-details-head .section-title{font-size:17px;line-height:1.15}.temperature-details-head .section-subtitle{margin-top:3px;font-size:10px}.temperature-details-actions{display:flex;align-items:center;justify-content:flex-end;gap:7px}.temperature-details-actions .temperature-window-switch{display:flex;gap:5px;flex-wrap:nowrap}.temperature-details-actions .temperature-window-switch button{min-width:48px;min-height:31px;padding:6px 8px;font-size:10px}.temperature-details-close{width:31px;min-width:31px;height:31px;padding:0;border-radius:9px;font-size:18px;line-height:1}.temperature-details-card #tempChart{width:100%;height:310px;margin-top:4px}.temperature-details-legend{justify-content:flex-start;gap:10px 15px;margin-top:6px;padding-top:8px;font-size:11px}@media (max-width:700px){.temperature-details-card{padding:13px 13px 11px}.temperature-details-card #tempChart{height:270px}}@media (max-width:520px){.temperature-details-head{align-items:flex-start}.temperature-details-actions{gap:5px}.temperature-details-actions .temperature-window-switch{gap:3px}.temperature-details-actions .temperature-window-switch button{min-width:40px;padding:5px 6px;font-size:9px}.temperature-details-card #tempChart{height:235px}.temperature-details-legend{font-size:9px}}.shot-profile-rail{padding-top:11px}.shot-profile-rail .home-profile-buttons,.shot-profile-rail .profile-rail-select{margin-top:0}.shot-chart-workspace{grid-template-columns:minmax(0,1fr) 270px;align-items:stretch;gap:12px;margin-top:9px}.shot-chart-plot{display:flex;flex-direction:column}.shot-profile-summary-in-chart{gap:6px;margin:0 0 7px}.shot-profile-summary-in-chart .shot-profile-chip{min-height:49px;padding:7px 8px;border-radius:10px}.shot-profile-summary-in-chart .shot-profile-chip small{overflow:hidden;font-size:8px;line-height:1.1;text-overflow:ellipsis;white-space:nowrap}.shot-profile-summary-in-chart .shot-profile-chip b{margin-top:3px;font-size:14px;line-height:1}.shot-chart-plot .home-quick-editor,.shot-chart-plot .main-curve-editor{margin-top:0;margin-bottom:7px}.profile-library{position:relative;top:auto;display:flex;min-height:100%;height:auto;flex-direction:column;padding:11px;overflow:hidden}.profile-library-section{min-width:0}.profile-library-title b{display:block;color:#e9edf3;font-size:12px;line-height:1.15}.profile-library-title small{display:block;margin-top:3px;color:#87919e;font-size:8px;line-height:1.2}.profile-library .home-profile-buttons{grid-template-columns:repeat(2,minmax(0,1fr));gap:5px;margin-top:8px}.profile-library .home-profile-buttons button{min-height:33px;padding:6px 7px;overflow:hidden;font-size:9px;text-align:center;text-overflow:ellipsis;white-space:nowrap}.profile-library .home-profile-buttons button:last-child{grid-column:1/-1}.profile-library-divider{height:1px;margin:11px 0;background:linear-gradient(90deg,transparent,#39424e 10%,#39424e 90%,transparent)}.user-profile-section{display:flex;min-height:0;flex:1 1 auto;flex-direction:column}.user-profile-save{display:grid;grid-template-columns:minmax(0,1fr) 34px;gap:5px;margin-top:8px}.user-profile-save input{min-width:0;height:34px;padding:6px 8px;border:1px solid #37424e;border-radius:9px;background:#11161b;color:#e7ebf1;font-size:10px;outline:0}.user-profile-save input:focus{border-color:#8b6544;box-shadow:0 0 0 2px rgba(225,139,72,.1)}.user-profile-save button{width:34px;min-width:34px;height:34px;padding:0;border-radius:9px;font-size:17px}.user-profile-list{display:flex;min-height:56px;max-height:235px;flex-direction:column;gap:5px;margin-top:7px;overflow:auto;scrollbar-width:thin}.user-profile-empty{display:grid;min-height:55px;place-items:center;padding:9px;border:1px dashed #35404b;border-radius:9px;color:#737e8b;font-size:var(--zone-font-small,12px);line-height:1.35;text-align:center}.user-profile-row{display:grid;grid-template-columns:minmax(0,1fr) auto;align-items:stretch;overflow:hidden;border:1px solid #303a46;border-radius:9px;background:#171c22}.user-profile-row.active{border-color:#9b673e;background:#2d241c;box-shadow:inset 0 0 0 1px rgba(225,139,72,.08)}.user-profile-apply{min-width:0;min-height:45px;padding:7px 8px;border:0;border-radius:0;background:0 0;color:#dce2e9;text-align:left;box-shadow:none}.user-profile-apply:hover:not(:disabled){background:#20262e}.user-profile-row.active .user-profile-apply:hover:not(:disabled){background:#35291f}.stop-weight-chip .stop-weight-copy b,.stop-weight-chip .stop-weight-copy small,.user-profile-apply b,.user-profile-apply small{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.user-profile-apply b{font-size:10px;line-height:1.15}.user-profile-apply small{margin-top:4px;color:#8994a1;font-size:7px;line-height:1.1}.user-profile-actions{display:grid;grid-template-columns:26px;border-left:1px solid #303a46}.user-profile-actions button{width:26px;min-width:26px;min-height:22px;padding:0;border:0;border-radius:0;background:0 0;color:#8f9aa7;font-size:10px;box-shadow:none}.user-profile-actions button+button{border-top:1px solid #303a46}.user-profile-actions button:hover:not(:disabled){background:#282f38;color:#e8edf3}.profile-library .profile-recipes-button{flex:0 0 auto;margin-top:9px}@media (max-width:800px){.shot-chart-workspace{grid-template-columns:minmax(0,1fr)}.profile-library{min-height:0}.profile-library .home-profile-buttons{grid-template-columns:repeat(5,minmax(0,1fr))}.profile-library .home-profile-buttons button:last-child{grid-column:auto}.user-profile-list{max-height:180px}}@media (max-width:600px){.shot-profile-summary-in-chart{grid-template-columns:repeat(5,minmax(84px,1fr));overflow-x:auto;padding-bottom:3px}.profile-library .home-profile-buttons{grid-template-columns:repeat(2,minmax(0,1fr))}.profile-library .home-profile-buttons button:last-child{grid-column:1/-1}}.stop-weight-chip{position:relative;display:grid;grid-template-columns:minmax(0,1fr) 31px;align-items:center;gap:6px;cursor:pointer;user-select:none}.stop-weight-copy{min-width:0}.stop-weight-switch{position:relative;display:block;width:31px;height:18px;flex:0 0 auto}.stop-weight-switch input{position:absolute;width:1px;height:1px;opacity:0;pointer-events:none}.stop-weight-switch i{position:absolute;inset:0;border:1px solid #46515e;border-radius:999px;background:#272e37;transition:background .18s ease,border-color .18s ease}.stop-weight-switch i::after{content:"";position:absolute;top:3px;left:3px;width:10px;height:10px;border-radius:50%;background:#8d98a5;transition:transform .18s ease,background .18s ease}.stop-weight-switch input:checked+i{border-color:#347052;background:#1e4a34}.stop-weight-switch input:checked+i::after{transform:translateX(13px);background:#92efb8}.stop-weight-chip.active{border-color:#347052;background:linear-gradient(145deg,#18281f,#111a15)}.stop-weight-chip.active b,.stop-weight-footer.active .stop-weight-copy b{color:#91eab5}.stop-weight-chip.disabled{opacity:.58;cursor:not-allowed}.profile-library .profile-recipes-button{display:none!important}@media (max-width:600px){.shot-profile-summary-in-chart{grid-template-columns:repeat(6,minmax(84px,1fr))}}.main-curve-editor{margin-top:0;margin-bottom:6px}.shot-chart-card.curve-dirty .main-curve-editor{border-color:#7d5a3e;background:linear-gradient(145deg,#231c17,#171410)}.shot-chart-card.curve-ready #chart{user-select:none}.chart-head-actions{margin-left:auto}.shot-profile-summary-in-chart .profile-chip-temperature{border-color:#684b35}.shot-profile-summary-in-chart .profile-chip-preinfusion{border-color:#315943;background:linear-gradient(145deg,rgba(34,61,46,.72),#111719)}.shot-profile-summary-in-chart .profile-chip-preinfusion:hover{border-color:#4b8062;background:linear-gradient(145deg,rgba(40,75,55,.82),#151d19)}.shot-profile-summary-in-chart .profile-chip-pause{border-color:#4c405f;background:linear-gradient(145deg,rgba(52,42,67,.72),#15131a)}.shot-profile-summary-in-chart .profile-chip-pause:hover{border-color:#715e89;background:linear-gradient(145deg,rgba(66,52,84,.82),#1a1620)}.shot-profile-summary-in-chart .profile-chip-main{border-color:#62522d;background:linear-gradient(145deg,rgba(66,55,29,.68),#18160f)}.shot-profile-summary-in-chart .profile-chip-main:hover{border-color:#8a733b;background:linear-gradient(145deg,rgba(83,68,34,.78),#201c11)}.shot-profile-summary-in-chart .profile-chip-weight{border-color:#394b62}.shot-chart-footer{display:flex;justify-content:space-between;gap:10px;min-height:43px;margin-top:7px;padding:6px 8px;border:1px solid #303844;border-radius:10px;background:#12171c}.shot-total-time{display:flex;min-width:0;align-items:baseline;gap:7px}.shot-total-time small{color:#8994a1;font-size:9px;font-weight:720}.shot-total-time b{color:#e4e9ef;font-size:14px;white-space:nowrap}.stop-weight-footer{display:grid;min-height:31px;grid-template-columns:minmax(0,1fr) 31px;align-items:center;gap:8px;padding:4px 7px 4px 9px;border:1px solid #37414d;border-radius:9px;background:#181d23;cursor:pointer;user-select:none}.stop-weight-footer .stop-weight-copy{display:flex;gap:6px}.stop-weight-footer .stop-weight-copy small,.target-weight-footer small{color:#939eaa;font-size:8px;white-space:nowrap}.stop-weight-footer .stop-weight-copy b{color:#cdd4dc;font-size:11px}.stop-weight-footer.active{border-color:#347052;background:#17251d}@media (max-width:600px){.shot-profile-summary-in-chart{grid-template-columns:repeat(5,minmax(84px,1fr))}.shot-chart-footer{align-items:stretch}}@media (max-width:430px){.shot-chart-footer{display:grid;grid-template-columns:minmax(0,1fr)}.stop-weight-footer{width:100%}}#userProfileHint.pending{color:#d7a36f}.user-profile-save input.pending{border-color:#76583e}.user-profile-save button.pending{border-color:#9b673e;box-shadow:0 0 0 2px rgba(225,139,72,.08)}.user-profile-save input.attention{border-color:#d66d6d;animation:user-profile-attention .34s ease 2}.user-profile-save button:disabled{opacity:.42;border-color:#37414d;box-shadow:none;cursor:not-allowed;filter:saturate(.55)}.user-profile-save button.pending:not(:disabled){opacity:1;border-color:#b77b49;box-shadow:0 0 0 2px rgba(225,139,72,.1),0 0 15px rgba(225,139,72,.14);filter:none}.shot-chart-card.curve-dirty #chart,.shot-chart-card.curve-editing #chart{opacity:1;visibility:visible}.home-profile-buttons button[data-profile=Custom].active{border-color:#b77b49;background:linear-gradient(145deg,#3a291d,#241b15);box-shadow:inset 0 0 0 1px rgba(225,139,72,.12),0 0 14px rgba(225,139,72,.1)}.shot-chart-card #chart{min-width:0;touch-action:none}.shot-profile-board{display:grid;min-width:0}.shot-profile-board>[data-shot-panel]{min-width:0;margin:0;transition:opacity .15s ease,transform .15s ease,border-color .15s ease}.shot-profile-card{min-height:0;height:auto;align-self:start;padding:14px}.profile-library-card-head{align-items:center;margin-bottom:11px;padding-bottom:10px;border-bottom:1px solid #303844}.profile-library-card-heading{min-width:0}.profile-library-card-head .section-title{font-size:18px;line-height:1.05}.profile-library-card-head .section-subtitle{display:flex;align-items:center;gap:6px;font-size:9px}.profile-count-badge{display:inline-grid;min-width:20px;height:18px;place-items:center;padding:0 5px;border:1px solid #3d4855;border-radius:999px;background:#1c2229;color:#cbd3dd}.profile-count-badge b{font-size:9px;line-height:1}.profile-card-actions{display:flex;flex:0 0 auto;align-items:center;gap:6px}.shot-panel-order-handle{display:grid;width:34px;min-width:34px;height:34px;place-items:center;padding:0;border-color:#46515e;border-radius:10px;background:#20262e;color:#aeb8c4;font-size:17px;line-height:1;cursor:grab;touch-action:none;user-select:none}.shot-panel-order-handle:hover{border-color:#756047;background:#2b241d;color:#efbc8d}.shot-panel-order-handle:active{cursor:grabbing;transform:scale(.96)}.shot-panel-dragging{opacity:.45;transform:scale(.992);border-color:#8b6544!important}body.home-top-panel-layout-dragging{user-select:none}body.shot-panel-layout-dragging{cursor:grabbing;user-select:none}.home-font-head>div,.shot-chart-plot{min-width:0}.shot-profile-card.profile-library{position:relative;top:auto;min-height:0;height:auto;overflow:hidden}@media (max-width:980px){.shot-profile-board,.shot-profile-board.profiles-first{grid-template-columns:minmax(0,1fr)}.shot-profile-board>[data-shot-panel]{width:100%}.shot-profile-card .home-profile-buttons{grid-template-columns:repeat(5,minmax(0,1fr))}.shot-profile-card .home-profile-buttons button:last-child{grid-column:auto}}@media (max-width:600px){.shot-profile-card .home-profile-buttons{grid-template-columns:repeat(2,minmax(0,1fr))}.shot-profile-card .home-profile-buttons button:last-child{grid-column:1/-1}.profile-library-card-head .section-title{font-size:16px}}@media (min-width:981px){.shot-profile-board{align-items:stretch}.shot-profile-board>[data-shot-panel]{align-self:stretch;height:100%}.shot-chart-card{height:100%}.shot-profile-card.profile-library{display:flex;height:100%;min-height:100%;flex-direction:column}.shot-profile-card .built-in-profile-section,.shot-profile-card .profile-library-card-head,.shot-profile-card .profile-library-divider,.shot-profile-card .profile-rail-select{flex:0 0 auto}.shot-profile-card .user-profile-section{min-height:0;flex:1 1 auto}.shot-profile-card .user-profile-list{min-height:120px;max-height:none;flex:1 1 auto}.shot-profile-card .user-profile-empty{min-height:120px;height:100%}}@media (max-width:980px){.shot-chart-card,.shot-profile-board>[data-shot-panel],.shot-profile-card.profile-library{height:auto;min-height:0;align-self:start}.shot-profile-card .user-profile-list{max-height:180px;flex:0 1 auto}.shot-profile-card .user-profile-empty{height:auto;min-height:55px}}.pressure-ramp-indicator{display:none!important}.shot-profile-summary-in-chart{grid-template-columns:repeat(4,minmax(0,1fr))}.stage-direct-chip{cursor:default;user-select:none}.stage-direct-chip:hover{border-color:#3c4552;background:linear-gradient(145deg,#171c23,#11151a)}.stage-direct-chip small::after{color:#697482;font-size:8px;font-weight:650}.shot-chart-footer{display:grid;grid-template-columns:auto minmax(125px,1fr) auto;align-items:center}.target-weight-footer{display:flex;min-width:0;min-height:31px;align-items:baseline;justify-content:center;gap:7px;padding:5px 10px;border:1px solid #37414d;border-radius:9px;background:#181d23;color:#dce2e9}.target-weight-footer:hover:not(:disabled){border-color:#526171;background:#20262e}.target-weight-footer b{color:#dce2e9;font-size:11px;white-space:nowrap}@media (max-width:600px){.shot-profile-summary-in-chart{grid-template-columns:repeat(4,minmax(86px,1fr))}.shot-chart-footer{grid-template-columns:minmax(0,1fr) auto}.target-weight-footer{grid-column:1/-1;grid-row:2;justify-content:flex-start}}@media (max-width:430px){.stage-direct-chip small::after{display:none}.shot-chart-footer{grid-template-columns:minmax(0,1fr)}.stop-weight-footer,.target-weight-footer{width:100%}}:root{--ha-font-family-body:Roboto, Noto, sans-serif;--ha-font-family-heading:var(--ha-font-family-body);--ha-font-size-xs:10px;--ha-font-size-s:12px;--ha-font-size-m:14px;--ha-font-size-l:16px;--ha-font-size-xl:20px;--ha-font-size-2xl:24px;--ha-font-size-3xl:28px;--ha-font-size-4xl:32px;--ha-font-size-5xl:40px;--ha-font-weight-light:300;--ha-font-weight-normal:400;--ha-font-weight-medium:500;--ha-font-weight-bold:700;--ha-line-height-condensed:1.2;--ha-line-height-normal:1.6;--ha-line-height-expanded:2}.home-font-head small,body,html{font-weight:var(--ha-font-weight-normal)}body,html{font-size:14px;line-height:var(--ha-line-height-normal);-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale;text-rendering:optimizeLegibility}.home-font-head b,button{font-weight:var(--ha-font-weight-medium)}[data-font-zone],body,button,html{font-family:var(--ha-font-family-body)}button,input,select,textarea{font-size:14px;line-height:var(--ha-line-height-condensed);-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale}input,select,textarea{font-family:var(--ha-font-family-body);font-weight:var(--ha-font-weight-normal)}[data-font-zone]{--zone-font-text:14px;--zone-font-small:12px;--zone-font-tiny:10px;--zone-font-title:20px;--zone-font-value:20px;--zone-font-chart:10px;font-size:var(--zone-font-text)}.typography-header-button span{display:block;min-width:22px;font-size:12px;font-weight:var(--ha-font-weight-bold);letter-spacing:-.04em;text-align:center}.home-font-tools{margin-bottom:11px;padding:12px;border:1px solid #3d4652;border-radius:13px;background:#151a20;font-size:14px}.home-font-tools[hidden]{display:none}.home-font-head,.home-font-window summary{display:flex;align-items:flex-start;justify-content:space-between;gap:12px}.home-font-head b{display:block;color:#e5e9ee;font-size:16px;line-height:1.2}.home-font-head small{display:block;margin-top:3px;color:#8994a0;font-size:12px;line-height:1.3}.ha-typography-preset-button{flex:0 0 auto;white-space:nowrap}.home-font-independent-note{margin-top:10px;padding:9px 10px;border-left:3px solid #54a9ff;border-radius:8px;background:rgba(84,169,255,.07);color:#b8c5d3;font-size:12px;line-height:1.35}.home-font-windows{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:9px;margin-top:11px}.home-font-window{min-width:0;border:1px solid #343d48;border-radius:11px;background:#11161b;overflow:hidden}.home-font-window[open]{border-color:#485361;background:#13191f}.home-font-window summary{min-height:48px;align-items:center;gap:10px;padding:9px 11px;cursor:pointer;list-style:none;user-select:none}.home-font-window summary::-webkit-details-marker{display:none}.home-font-window summary::before{content:"›";flex:0 0 auto;color:#7f8b98;font-size:20px;line-height:1;transform-origin:center;transition:transform .16s ease}.home-font-window[open] summary::before{transform:rotate(90deg)}.home-font-window summary>span{min-width:0;flex:1 1 auto}.home-font-window summary b{display:block;overflow:hidden;color:#dce2e9;font-size:14px;text-overflow:ellipsis}.home-font-window summary small{display:block;margin-top:2px;color:#778390;font-size:10px}.home-font-window summary>output{flex:0 0 auto;color:#aeb8c3;font-family:ui-monospace,SFMono-Regular,Consolas,monospace;font-size:10px;white-space:nowrap}.home-font-window-controls{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px;padding:0 10px 10px}.home-font-control{min-width:0;padding:9px 10px;border:1px solid #303945;border-radius:9px;background:#0f1419}.home-font-control>span{display:flex;align-items:baseline;justify-content:space-between;gap:8px}.home-font-control b,.home-font-control output,.home-font-window summary b{font-weight:var(--ha-font-weight-medium);white-space:nowrap}.home-font-control b{overflow:hidden;color:#bfc7d0;font-size:11px;text-overflow:ellipsis}.home-font-control output{color:#e5e9ee;font-size:12px}.home-font-control input[type=range]{width:100%;margin-top:8px;accent-color:#728296}.shot-line-settings{margin:0 10px 10px;padding:10px;border:1px solid #303945;border-radius:10px;background:#0e1318}.shot-line-settings-head{display:flex;align-items:center;justify-content:space-between;gap:10px;margin-bottom:8px}.shot-line-settings-head>span{min-width:0}.shot-line-settings-head b,.shot-line-settings-head small{display:block}.shot-line-settings-head b{color:#dbe1e8;font-size:12px;font-weight:var(--ha-font-weight-medium)}.shot-line-settings-head small{margin-top:2px;color:#7f8a97;font-size:10px}.shot-line-settings-head button{flex:0 0 auto}.shot-line-settings-grid{display:grid;gap:6px}.shot-line-setting-row{display:grid;grid-template-columns:minmax(90px,.55fr) minmax(115px,1fr) minmax(115px,1fr);align-items:center;gap:8px;padding:8px;border:1px solid #29323c;border-radius:9px;background:#11171c}.shot-line-setting-name{min-width:0;display:flex;align-items:center;gap:7px}.shot-line-setting-name i{width:22px;height:0;flex:0 0 auto;border-top:3px solid currentColor}.shot-line-setting-name i.dashed{border-top-style:dashed}.shot-line-control output,.shot-line-setting-name b{font-weight:var(--ha-font-weight-medium);white-space:nowrap}.shot-line-setting-name b{overflow:hidden;color:#cbd3dc;font-size:11px;text-overflow:ellipsis}.shot-line-control{min-width:0}.shot-line-control>span{display:flex;align-items:baseline;justify-content:space-between;gap:7px;color:#8f9aa6;font-size:9px}.shot-line-control output{color:#dce2e8;font-size:10px}.shot-line-control input[type=range]{width:100%;margin-top:6px;accent-color:#728296}@media (max-width:620px){.shot-line-setting-row{grid-template-columns:minmax(0,1fr)}.shot-line-settings-head{align-items:flex-start}}.home-font-window-actions{display:flex;justify-content:flex-end;padding:0 10px 10px}.home-font-section-title{display:flex;align-items:baseline;gap:8px;margin-top:14px;padding-top:11px;border-top:1px solid #303944;color:#d8dde3}.home-font-section-title b{font-size:14px;font-weight:var(--ha-font-weight-medium)}.home-font-section-title small{color:#7f8a97;font-size:10px;font-weight:var(--ha-font-weight-normal)}.home-font-transfer{width:100%;min-height:76px;margin-top:9px;padding:9px 10px;resize:vertical;border:1px solid #3a4450;border-radius:10px;background:#0f1419;color:#cfd6de;font-family:ui-monospace,SFMono-Regular,Consolas,monospace;font-size:11px;line-height:1.35;overflow-wrap:anywhere}.home-font-transfer:focus{border-color:#66778a;outline:2px solid rgba(102,119,138,.18)}.home-font-transfer-actions{display:flex;justify-content:flex-end;flex-wrap:wrap;gap:7px;margin-top:8px}.home-font-summary{margin-top:9px;color:#7f8a97;font-size:10px;line-height:1.2;text-align:right}#page-settings .settings-fields .setting input,#page-settings .settings-fields .setting select,#sidebar .nav-label{font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-normal)}#sidebar .side-brand{font-size:var(--zone-font-title)!important;font-weight:var(--ha-font-weight-medium)}#sidebar .side-brand small,#sidebar .side-footer{font-size:var(--zone-font-small)!important}#sidebar .nav-icon{font-size:var(--zone-font-value)!important}.machine-control-card,.machine-control-card button{font-size:var(--zone-font-text)!important}.machine-control-card .machine-control-title{font-size:var(--zone-font-title)!important;font-weight:var(--ha-font-weight-normal)}#shotChartCard .shot-profile-chip b,#shotChartCard .shot-total-time b,#shotChartCard .stop-weight-footer b,#shotChartCard .target-weight-footer b,#temperatureDetailsCard .section-title,.machine-control-card .machine-auto-off b,.machine-control-card .machine-mode-status b,.machine-control-card .machine-state{font-size:var(--zone-font-value)!important;font-weight:var(--ha-font-weight-medium)}.home-metrics-panel .home-metric-copy small,.home-metrics-panel .home-metric-state,.machine-control-card .machine-water-status b,.machine-control-card .quick-action small{font-size:var(--zone-font-small)!important}#page-maintenance .service-fields .setting input,#page-maintenance .service-fields .setting select,#profileLibraryCard,#profileLibraryCard button,#profileLibraryCard input,#profileLibraryCard select,#shotChartCard,#shotChartCard button,#shotChartCard input,#temperatureDetailsCard,#temperatureDetailsCard button,.home-metrics-panel,.home-metrics-panel .home-metric-copy>span,.machine-control-card .quick-action b{font-size:var(--zone-font-text)!important}.home-metrics-panel .home-metric-copy strong{font-size:var(--zone-font-value)!important;font-weight:var(--ha-font-weight-normal)}#temperatureDetailsCard .section-title{font-size:var(--zone-font-title)!important}#temperatureDetailsCard .legend,#temperatureDetailsCard .section-subtitle{font-size:var(--zone-font-chart)!important}#shotChartCard .section-title{font-size:var(--zone-font-title)!important;font-weight:var(--ha-font-weight-medium)}#shotChartCard .legend,#shotChartCard .section-subtitle,#shotChartCard .shot-profile-chip small,#shotChartCard .shot-total-time small,#shotChartCard .stop-weight-footer small,#shotChartCard .target-weight-footer small{font-size:var(--zone-font-chart)!important}#profileLibraryCard .profile-library-title b,#profileLibraryCard .section-title{font-size:var(--zone-font-title)!important;font-weight:var(--ha-font-weight-medium)}#profileLibraryCard .profile-rail-select span,#profileLibraryCard .section-subtitle,#profileLibraryCard small{font-size:var(--zone-font-small)!important}#profileLibraryCard .profile-count-badge b,#profileLibraryCard .user-profile-count{font-size:var(--zone-font-value)!important}.stage-direct-chip small::after{content:"";display:none}@media (max-width:900px){.home-font-windows{grid-template-columns:minmax(0,1fr)}}@media (max-width:620px){.home-font-window-controls{grid-template-columns:minmax(0,1fr)}.home-font-head{display:block}.ha-typography-preset-button{margin-top:9px}.home-font-section-title{display:block}.home-font-section-title small{display:block;margin-top:2px}.home-font-transfer-actions{display:grid;grid-template-columns:minmax(0,1fr)}.home-font-transfer-actions button{width:100%}.home-font-summary{text-align:left}}.profile-main-title,.profile-main-title>span{display:block}.profile-main-title>small{display:block;margin-top:3px;color:#8d98a5;font-size:var(--zone-font-small, 12px);font-weight:var(--ha-font-weight-normal, 400);line-height:1.2}.user-profile-title>b{display:flex;align-items:center;gap:6px}.user-profile-title .profile-count-badge{display:inline-grid;flex:0 0 auto}.profile-count-badge span{font-size:9px;line-height:1}#homePidState[hidden],#homeStopByWeightState[hidden],body.sidebar-collapsed .nav-item::after,body.sidebar-collapsed .nav-item::before{display:none!important}.stop-weight-footer .stop-weight-copy{align-items:center}.sidebar .nav-item{border:0;box-shadow:none}.sidebar .nav-item:focus-visible{outline:2px solid rgba(79,195,247,.75);outline-offset:1px}@media (hover:hover){body.sidebar-collapsed .nav-item:hover:not(.active){background:#252b33;color:#e3e8ee}}.history-page-head{display:flex;align-items:center;justify-content:space-between;gap:14px;margin:2px 0 12px}.history-page-head p,.settings-page-head p{margin:3px 0 0;color:var(--muted);font-size:12px}.history-refresh-button{width:38px;min-width:38px;height:38px;display:grid;place-items:center;padding:0;border:1px solid #343c47;border-radius:10px;background:#20252d;color:#cbd3dc}.history-refresh-button svg{width:19px;height:19px;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}.history-workspace{display:grid;grid-template-columns:minmax(255px,.36fr) minmax(0,.64fr);gap:12px;align-items:start}.history-list-card{padding:10px;min-height:420px;max-height:calc(100vh - 112px);overflow:hidden}.history-list-head{display:flex;align-items:center;justify-content:space-between;gap:10px;padding:5px 6px 10px}.history-list-head b{font-size:14px}.history-list-head small{color:var(--muted);font-size:10px}.history-list{display:grid;gap:5px;max-height:calc(100vh - 170px);overflow:auto;padding-right:2px;scrollbar-width:thin}.history-list-state{display:grid;place-items:center;min-height:150px;padding:18px;color:var(--muted);text-align:center;font-size:12px}.history-row,.home-shot-rail-tabs button{min-width:0;align-items:center;background:0 0}.history-row{width:100%;display:grid;grid-template-columns:48px minmax(0,1fr) auto;gap:9px;min-height:58px;padding:8px 9px;border:1px solid transparent;border-radius:11px;color:#d6dce4;text-align:left}.history-row:hover{background:#20252d;border-color:#303844}.history-row.active{background:#252b34;border-color:#46515f;box-shadow:none}.history-row-time{color:#aab3be;font-size:11px;font-weight:700;white-space:nowrap}.history-row-copy{min-width:0;display:grid;gap:2px}.history-row-copy b,.history-row-copy small{overflow:hidden;color:#eef1f5;font-size:12px;font-weight:720;text-overflow:ellipsis;white-space:nowrap}.history-row-copy small{color:#8e98a5;font-size:9px;font-weight:550}.history-row-arrow{color:#707b88;font-size:18px;line-height:1}.history-row.active .history-row-arrow{color:#d0d7df}.history-detail-card{position:sticky;top:14px;min-height:420px;padding:14px}.history-detail-empty{min-height:390px;display:grid;place-items:center;align-content:center;gap:8px;color:#828c98;text-align:center}.history-detail-empty svg,.home-archive-rail-head svg,.settings-nav-icon svg{width:38px;height:38px;fill:none;stroke:#6d7885;stroke-width:1.5;stroke-linecap:round;stroke-linejoin:round}.history-detail-empty b{color:#cbd2da;font-size:14px}.history-detail-empty span{font-size:11px}.history-detail-head{display:flex;align-items:flex-start;justify-content:space-between;gap:12px;margin-bottom:12px}.history-detail-head .section-subtitle{margin-top:3px}.history-show-home{flex:0 0 auto;min-height:34px;padding:7px 10px;border:1px solid #3a4552;border-radius:9px;background:#252b34;color:#dce2e9;font-size:10px}.history-detail-metrics{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:7px;margin-bottom:10px}.history-detail-metrics>div{min-width:0;padding:8px 9px;border:1px solid #303844;border-radius:10px;background:#12171c}.history-detail-metrics b,.history-detail-metrics small{display:block;overflow:hidden;color:#8e98a5;font-size:8px;text-overflow:ellipsis;white-space:nowrap}.history-detail-metrics b{margin-top:3px;color:#e7ebf0;font-size:13px}.history-chart-wrap{min-width:0;padding-top:4px}#historyChart{display:block;width:100%;height:305px}.history-legend{margin-top:2px}.history-detail-note{min-height:16px;margin-top:8px;color:#818b97;font-size:9px;line-height:1.45}@media (max-width:820px){.history-workspace{grid-template-columns:minmax(0,1fr)}.history-list-card{min-height:0;max-height:none}.history-list{max-height:360px}.history-detail-card{position:static;min-height:400px}}@media (max-width:520px){.history-detail-head{display:grid}.history-show-home{width:100%}.history-detail-metrics{grid-template-columns:repeat(2,minmax(0,1fr))}#historyChart{height:270px}}#historyDetailBody[hidden],.history-detail-empty[hidden]{display:none!important}.shot-profile-board,.shot-profile-board.profiles-first{grid-template-columns:minmax(255px,.36fr) minmax(0,.64fr);grid-template-areas:"rail chart";align-items:stretch;gap:12px}#profileLibraryCard{grid-area:rail}#shotChartCard{grid-area:chart}.shot-profile-board>[data-shot-panel]{align-self:stretch;height:100%}.home-shot-rail,.shot-chart-card{min-height:520px}.home-shot-rail{display:flex!important;flex-direction:column;padding:10px!important;overflow:hidden}.home-shot-rail-tabs{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:4px;padding:3px;border:1px solid #303946;border-radius:11px;background:#11161b}.home-shot-rail-tabs button{min-height:34px;display:flex;justify-content:center;gap:6px;padding:7px 8px;border-radius:8px;color:#8994a1;font-size:11px;font-weight:650;box-shadow:none}.home-shot-rail-tabs button.active{background:#29303a;color:#eef2f6}.home-shot-tab-count{display:grid;min-width:18px;height:17px;place-items:center;padding:0 4px;border:1px solid #46515e;border-radius:999px;color:#b9c2cc;font-size:8px;line-height:1}.home-shot-rail-tabs button.active .home-shot-tab-count{border-color:#5b6877;color:#eef2f6}.home-shot-rail-pane{min-height:0;flex:1 1 auto;margin-top:9px}.home-shot-rail-pane[hidden]{display:none!important}.home-archive-rail-pane,.home-profile-rail-pane{display:flex;min-height:0;flex-direction:column}.home-profile-rail-pane .built-in-profile-section,.home-profile-rail-pane .profile-library-card-head,.home-profile-rail-pane .profile-library-divider,.home-profile-rail-pane .profile-rail-select{flex:0 0 auto}.home-profile-rail-pane .user-profile-section{min-height:0;flex:1 1 auto}.home-profile-rail-pane .user-profile-list{min-height:110px;max-height:none;flex:1 1 auto}.home-archive-rail-head{display:flex;flex:0 0 auto;align-items:center;justify-content:space-between;gap:10px;padding:5px 5px 9px;border-bottom:1px solid #303844}.home-archive-rail-head>div{min-width:0}.home-archive-rail-head b,.home-archive-rail-head small{display:block;overflow:hidden;color:#e1e6ec;font-size:12px;text-overflow:ellipsis;white-space:nowrap}.home-archive-rail-head small{margin-top:2px;color:#7f8a97;font-size:9px}.home-archive-rail-head button{width:31px;min-width:31px;height:31px;display:grid;place-items:center;padding:0;border:1px solid #3b4552;border-radius:9px;background:#20262e;color:#aab5c1}.home-archive-rail-head svg,.settings-nav-icon svg{width:16px;height:16px;stroke:currentColor;stroke-width:1.8}.home-archive-list{min-height:0;max-height:none;flex:1 1 auto;margin-top:7px;overflow:auto}.home-archive-list .history-row{min-height:55px;grid-template-columns:44px minmax(0,1fr) auto;padding:7px 8px}.home-archive-summary{grid-template-columns:repeat(4,minmax(0,1fr));gap:7px;margin-bottom:8px}.home-archive-summary[hidden],.shot-chart-card.archive-view #curveEditHint,.shot-chart-card.archive-view #shotProfileSummary,.shot-chart-card.archive-view .shot-chart-footer{display:none!important}.home-archive-metric{min-width:0;padding:8px 9px;border:1px solid #34404c;border-radius:10px;background:#12171c}.home-archive-metric b,.home-archive-metric small{display:block;overflow:hidden;color:#8e98a5;font-size:8px;text-overflow:ellipsis;white-space:nowrap}.home-archive-metric b{margin-top:3px;color:#e7ebf0;font-size:13px}.shot-chart-card.archive-view #homeArchiveSummary{display:grid!important}.shot-chart-card.archive-view #chart{touch-action:auto}@media (max-width:980px){.shot-profile-board,.shot-profile-board.profiles-first{grid-template-columns:minmax(0,1fr);grid-template-areas:"rail""chart"}.home-shot-rail,.shot-chart-card{min-height:0;height:auto}.home-archive-list{max-height:360px}}@media (max-width:600px){.home-archive-summary{grid-template-columns:repeat(2,minmax(0,1fr))}}.settings-page-head{margin:2px 0 12px}.settings-workspace{display:grid;grid-template-columns:minmax(225px,.3fr) minmax(0,.7fr);gap:12px;align-items:stretch}.settings-detail-card,.settings-nav-card{min-height:430px}.settings-nav-card{padding:10px}.settings-nav-head{display:flex;align-items:center;justify-content:space-between;gap:10px;padding:5px 6px 10px}.settings-nav-head b{color:#e2e7ed;font-size:13px}.settings-nav-head small{color:#7f8995;font-size:9px}.settings-nav-list{display:grid;gap:5px}.settings-nav-item{width:100%;min-width:0;min-height:58px;display:grid;grid-template-columns:30px minmax(0,1fr) auto;align-items:center;gap:9px;padding:8px 9px;border:1px solid transparent;border-radius:11px;background:0 0;color:#aab4c0;text-align:left;box-shadow:none}.settings-nav-item:hover{border-color:#303945;background:#20252d}.settings-nav-item.active{border-color:#46515f;background:#252b34;color:#eef2f6}.settings-nav-icon{width:30px;height:30px;display:grid;place-items:center;border-radius:9px;background:#20262e;color:#8f9ba8}.service-nav-item.active .service-nav-icon,.settings-nav-item.active .settings-nav-icon{background:#343c47;color:#e5eaf0}.settings-nav-icon svg{width:18px;height:18px;stroke-width:1.7}.settings-nav-item>span:nth-child(2){min-width:0}.settings-nav-item b,.settings-nav-item small{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.settings-nav-item b{color:inherit;font-size:11px;font-weight:700}.settings-nav-item small{color:#7e8996}.settings-nav-item>i{color:#67727f;font-size:17px;font-style:normal}.service-nav-item.active>i,.settings-nav-item.active>i{color:#cbd3dc}.settings-detail-card{padding:16px}.settings-panel[hidden]{display:none!important}.settings-panel-head{display:flex;align-items:flex-start;justify-content:space-between;gap:12px;padding-bottom:11px;border-bottom:1px solid #303844}.settings-panel-head .section-title{font-size:17px}.settings-panel-head .section-subtitle{margin-top:4px;font-size:10px}.settings-fields{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px;margin-top:11px}.settings-fields .setting,.settings-fields .toggle-setting{margin:0;padding:9px 10px;border:1px solid #303944;border-radius:10px;background:#12171c}.settings-fields .setting{min-width:0;display:grid;gap:6px}.settings-fields .setting label{overflow:hidden;color:#8e98a5;font-size:9px;font-weight:650;text-overflow:ellipsis;white-space:nowrap}.settings-fields .setting input,.settings-fields .setting select{width:100%;min-width:0;min-height:35px;padding:7px 9px;border-radius:8px;font-size:12px}.settings-wide{grid-column:1/-1}.settings-wide-half{grid-column:auto}.settings-fields .toggle-setting{min-height:58px}.settings-fields .toggle-copy span{font-size:10px}.settings-fields .toggle-copy small,.settings-nav-item small{margin-top:2px;font-size:8px}.settings-fields details.advanced{margin:0;border-color:#303944;border-radius:10px;background:#12171c}.settings-fields details.advanced summary{padding:10px;font-size:10px}.settings-advanced-fields{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px;padding:0 9px 9px}.settings-subsection{margin-top:13px}.settings-subsection+.settings-subsection{padding-top:13px;border-top:1px solid #252d36}.settings-subsection-head{display:flex;align-items:baseline;justify-content:space-between;gap:12px}.settings-subsection-head b{color:#dce2e9;font-size:12px;font-weight:700}.settings-subsection-head small{color:#778390;font-size:9px;text-align:right}.settings-fields-three{grid-template-columns:repeat(3,minmax(0,1fr))}.service-status-grid>div,.settings-status-field,.settings-status-grid>div,.settings-status-inline{min-width:0;display:flex;align-items:center;justify-content:space-between;gap:9px;border:1px solid #303944}.settings-status-field,.settings-status-inline{padding:9px 10px;border-radius:10px;background:#12171c}.settings-status-field span,.settings-status-inline span{color:#8e98a5;font-size:9px}.settings-status-field b,.settings-status-inline b{color:#dce2e9;font-size:10px;font-weight:700;white-space:nowrap}.settings-actions-split{align-items:center;justify-content:space-between}.settings-actions-split .settings-status-inline{flex:1 1 230px;max-width:360px}.settings-actions{display:flex;justify-content:flex-end;flex-wrap:wrap;gap:7px;margin-top:11px}.settings-actions button{min-height:34px;padding:7px 11px;font-size:10px}.settings-status-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:7px;margin-top:11px}.service-status-grid>div,.settings-status-grid>div{padding:8px 9px;border-radius:9px;background:#11161b}.settings-status-grid b,.settings-status-grid span{overflow:hidden;color:#8994a0;font-size:9px;text-overflow:ellipsis;white-space:nowrap}.settings-status-grid b{color:#dce2e9;font-size:10px;font-weight:700}.settings-status-single{grid-template-columns:minmax(0,1fr);max-width:320px;margin-left:auto}.settings-note{margin-top:11px;padding:8px 9px;border-left:3px solid #44515f;border-radius:8px;background:#141a20;color:#7f8995;font-size:9px;line-height:1.4}@media (max-width:760px){.settings-workspace{grid-template-columns:minmax(0,1fr)}.settings-detail-card,.settings-nav-card{min-height:0}.settings-nav-card{overflow:hidden}.settings-nav-head{padding-bottom:7px}.settings-nav-list{display:flex;gap:6px;overflow-x:auto;padding-bottom:2px;scrollbar-width:thin}.settings-nav-item{min-width:175px}}@media (max-width:520px){.settings-detail-card{padding:12px}.settings-advanced-fields,.settings-fields,.settings-fields-three,.settings-status-grid{grid-template-columns:minmax(0,1fr)}.settings-subsection-head{display:grid;gap:3px}.settings-subsection-head small{text-align:left}.settings-wide,.settings-wide-half{grid-column:1}.settings-status-single{max-width:none}.settings-actions{display:grid;grid-template-columns:minmax(0,1fr)}.settings-actions button{width:100%}}#page-settings,#page-settings button,#page-settings input,#page-settings select{font-family:var(--ha-font-family-body);font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-normal)}#page-settings .settings-nav-head b,#page-settings .settings-page-head h1,#page-settings .settings-panel-head .section-title{font-size:var(--zone-font-title)!important;font-weight:var(--ha-font-weight-medium);line-height:var(--ha-line-height-condensed)}#page-settings .settings-fields .setting label,#page-settings .settings-fields .toggle-copy small,#page-settings .settings-nav-head small,#page-settings .settings-nav-item small,#page-settings .settings-note,#page-settings .settings-page-head p,#page-settings .settings-panel-head .section-subtitle{font-size:var(--zone-font-small)!important;font-weight:var(--ha-font-weight-normal);line-height:1.3}#page-settings .settings-actions button,#page-settings .settings-fields .toggle-copy span,#page-settings .settings-fields details.advanced summary,#page-settings .settings-nav-item b{font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);line-height:var(--ha-line-height-condensed)}#page-settings .settings-status-grid span{font-size:var(--zone-font-small)!important;font-weight:var(--ha-font-weight-normal)}#page-settings .settings-status-grid b,#page-settings .settings-status-grid b i{font-size:var(--zone-font-value)!important;font-weight:var(--ha-font-weight-medium)}#page-diagnostics .service-nav-item>i,#page-maintenance .service-nav-item>i,#page-settings .settings-nav-item>i{font-size:var(--zone-font-title)!important}#page-settings .settings-nav-icon{color:#8f9ba8}#page-settings .settings-nav-icon svg,.service-nav-icon svg{width:20px;height:20px}#page-settings [data-settings-panel=temperature]{width:100%;margin:0 auto}.temperature-settings-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:10px;margin-top:12px}.temperature-settings-section{min-width:0;padding:12px;border:1px solid #303944;border-radius:12px;background:#12171c}.temperature-settings-section-head{display:grid;gap:3px;margin-bottom:9px}#page-settings .pid-parameter-main label,#page-settings .temperature-setting-copy>b,#page-settings .temperature-setting-copy>label,#page-settings .temperature-settings-section-head b{color:#dce2e9;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);line-height:1.25}#page-settings .pid-parameter-card>small,#page-settings .temperature-setting-copy small,#page-settings .temperature-settings-section-head small{color:#84909d;font-size:var(--zone-font-small)!important;font-weight:var(--ha-font-weight-normal);line-height:1.35}.pid-parameter-grid{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px}.pid-parameter-card{min-width:0;display:grid;gap:7px;padding:9px 10px;border:1px solid #2b343e;border-radius:10px;background:#171d23}.pid-parameter-main{display:flex;align-items:center;justify-content:space-between;gap:10px}#page-settings .pid-parameter-main input,#page-settings .temperature-setting-control input{width:132px;min-width:0;min-height:36px;padding:7px 9px;border-radius:8px;font-size:var(--zone-font-text)!important;font-variant-numeric:tabular-nums}.temperature-autotune-row,.temperature-setting-row{min-width:0;display:grid;align-items:center;gap:10px;padding:9px 0;border-top:1px solid #29313a}.temperature-setting-row{grid-template-columns:minmax(0,1fr) 138px}.temperature-autotune-row{grid-template-columns:minmax(0,1fr) auto auto;margin-top:9px;padding-bottom:0}.temperature-settings-section-head+.temperature-setting-row{border-top:0}.temperature-status-row{grid-template-columns:minmax(0,1fr) auto}.temperature-setting-copy{min-width:0;display:grid;gap:3px}.temperature-setting-control{display:grid;grid-template-columns:minmax(0,1fr) auto;align-items:center;gap:7px;min-width:0}#page-settings .temperature-setting-control input{width:100%}#page-settings .temperature-setting-control span{color:#9ba6b2;font-size:var(--zone-font-small)!important}#page-settings .temperature-autotune-row>strong,#page-settings .temperature-status-row>strong{color:#dce2e9;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);white-space:nowrap}#page-settings .temperature-autotune-row button{min-height:36px;padding:7px 12px;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium)}.temperature-settings-save{margin-top:10px}@media (max-width:760px){#page-settings [data-settings-panel=temperature]{max-width:none}.pid-parameter-grid,.temperature-settings-grid{grid-template-columns:minmax(0,1fr)}.temperature-settings-section-wide{grid-column:1}.pid-parameter-main{display:grid;grid-template-columns:minmax(0,1fr) minmax(120px,160px)}#page-settings .pid-parameter-main input{width:100%}}@media (max-width:520px){.temperature-settings-section{padding:10px}.temperature-autotune-row,.temperature-setting-row{grid-template-columns:minmax(0,1fr)}.temperature-setting-control{grid-template-columns:minmax(0,1fr) auto}#page-settings .temperature-autotune-row button{width:100%}}.service-page-head{margin:2px 0 12px}.service-page-head h1{margin:0}.service-page-head p{margin:3px 0 0;color:var(--muted)}.service-workspace{display:grid;grid-template-columns:minmax(225px,.3fr) minmax(0,.7fr);gap:12px;align-items:stretch}.service-detail-card,.service-nav-card{min-height:430px}.service-nav-card{padding:10px}.service-nav-head{display:flex;align-items:center;justify-content:space-between;gap:10px;padding:5px 6px 10px}.service-nav-list{display:grid;gap:5px}.service-nav-item{width:100%;min-width:0;min-height:58px;display:grid;grid-template-columns:30px minmax(0,1fr) auto;align-items:center;gap:9px;padding:8px 9px;border:1px solid transparent;border-radius:11px;background:0 0;color:#aab4c0;text-align:left;box-shadow:none}.service-nav-item:hover{border-color:#303945;background:#20252d}.service-nav-item.active{border-color:#46515f;background:#252b34;color:#eef2f6}.service-nav-icon{width:30px;height:30px;display:grid;place-items:center;border-radius:9px;background:#20262e;color:#8f9ba8}.service-nav-icon svg{fill:none;stroke:currentColor;stroke-width:1.7;stroke-linecap:round;stroke-linejoin:round}.service-nav-item>span:nth-child(2){min-width:0}.service-nav-item b,.service-nav-item small{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.service-nav-item small{margin-top:2px;color:#7e8996}.service-nav-item>i{color:#67727f;font-style:normal}.service-detail-card{padding:16px}.service-panel[hidden]{display:none!important}.service-panel-head{padding-bottom:11px;border-bottom:1px solid #303844}.service-panel-head .section-subtitle{margin-top:4px}.service-fields{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:8px;margin-top:11px}.service-fields .setting{min-width:0;display:grid;gap:6px;margin:0}.service-fields .setting input,.service-fields .setting select{width:100%;min-width:0;min-height:35px;padding:7px 9px;border-radius:8px}.diagnostics-metrics>div,.service-fields .setting,.service-fields .toggle-setting{padding:9px 10px;border:1px solid #303944;border-radius:10px;background:#12171c}.service-fields .toggle-setting{min-height:58px;margin:0}.service-wide{grid-column:1/-1}.service-actions{display:flex;justify-content:flex-end;flex-wrap:wrap;gap:7px;margin-top:11px}.service-actions .button,.service-actions button{min-height:34px;padding:7px 11px}.service-actions-danger{padding-top:11px;border-top:1px solid #303844}.service-status-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:7px;margin-top:11px}.diagnostics-metrics b,.diagnostics-metrics span,.service-status-grid b,.service-status-grid span{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.service-status-wide{grid-column:1/-1}.service-status-single{grid-template-columns:minmax(0,1fr);max-width:320px;margin-left:auto}.service-note{margin-top:11px;padding:8px 9px;border-left:3px solid #44515f;border-radius:8px;background:#141a20;color:#7f8995;line-height:1.4}.diagnostics-metrics{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:8px;margin-top:11px}.diagnostics-regulator-grid{grid-template-columns:repeat(2,minmax(0,1fr))}.diagnostics-metrics>div{min-width:0;min-height:68px;display:grid;align-content:center;gap:4px}.diagnostics-wide{grid-column:1/-1}#page-diagnostics,#page-diagnostics .button,#page-diagnostics button,#page-diagnostics input,#page-diagnostics select,#page-maintenance,#page-maintenance .button,#page-maintenance button,#page-maintenance input,#page-maintenance select{font-family:var(--ha-font-family-body);font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-normal)}#page-diagnostics .service-nav-head b,#page-diagnostics .service-page-head h1,#page-diagnostics .service-panel-head .section-title,#page-maintenance .service-nav-head b,#page-maintenance .service-page-head h1,#page-maintenance .service-panel-head .section-title{font-size:var(--zone-font-title)!important;font-weight:var(--ha-font-weight-medium);line-height:var(--ha-line-height-condensed)}#page-diagnostics .diagnostics-metrics span,#page-diagnostics .service-nav-head small,#page-diagnostics .service-nav-item small,#page-diagnostics .service-page-head p,#page-diagnostics .service-panel-head .section-subtitle,#page-maintenance .service-fields .setting label,#page-maintenance .service-fields .toggle-copy small,#page-maintenance .service-nav-head small,#page-maintenance .service-nav-item small,#page-maintenance .service-note,#page-maintenance .service-page-head p,#page-maintenance .service-panel-head .section-subtitle,#page-maintenance .service-status-grid span{font-size:var(--zone-font-small)!important;font-weight:var(--ha-font-weight-normal);line-height:1.3}#page-diagnostics .service-actions .button,#page-diagnostics .service-nav-item b,#page-maintenance .service-actions .button,#page-maintenance .service-actions button,#page-maintenance .service-fields .toggle-copy span,#page-maintenance .service-nav-item b{font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);line-height:var(--ha-line-height-condensed)}#page-diagnostics .diagnostics-metrics b,#page-diagnostics .diagnostics-metrics b i,#page-maintenance .service-status-grid b,#page-maintenance .service-status-grid b i{font-size:var(--zone-font-value)!important;font-weight:var(--ha-font-weight-medium)}@media (max-width:760px){.service-workspace{grid-template-columns:minmax(0,1fr)}.service-detail-card,.service-nav-card{min-height:0}.service-nav-list{display:flex;gap:6px;overflow-x:auto;padding-bottom:2px;scrollbar-width:thin}.service-nav-item{min-width:175px}}@media (max-width:620px){.diagnostics-metrics{grid-template-columns:repeat(2,minmax(0,1fr))}.diagnostics-wide{grid-column:1/-1}}@media (max-width:520px){.service-detail-card{padding:12px}.diagnostics-metrics,.service-fields,.service-status-grid{grid-template-columns:minmax(0,1fr)}.diagnostics-wide,.service-status-wide,.service-wide{grid-column:1}.service-status-single{max-width:none}.service-actions{display:grid;grid-template-columns:minmax(0,1fr)}.service-actions .button,.service-actions button{width:100%;text-align:center}}@media (min-width:841px){.shell{width:100%;max-width:none;margin:0;padding:14px 14px 14px 0}.sidebar{border-radius:0 18px 18px 0}body.sidebar-collapsed .sidebar{border-radius:0 15px 15px 0}body.sidebar-collapsed .content{transform:none}}@media (min-width:1186px){body.sidebar-collapsed .content{transform:translateX(-34.5px)}}.header-language-switch{display:grid;grid-template-columns:repeat(2,32px);gap:3px;padding:3px;border:1px solid #343e4a;border-radius:10px;background:#171d24}.header-language-switch button{width:32px;min-width:32px;height:28px;min-height:28px;padding:0;border-radius:7px;background:0 0;color:#85909d;font-size:10px;font-weight:700;letter-spacing:.04em;box-shadow:none}.header-language-switch button:hover{background:#242b34;color:#d3dae2}.header-language-switch button.active{background:#303844;color:#f0f3f6;box-shadow:inset 0 0 0 1px rgba(255,255,255,.035),0 2px 7px rgba(0,0,0,.18)}@media (max-width:520px){.header-language-switch{grid-template-columns:repeat(2,29px)}.header-language-switch button{width:29px;min-width:29px}}.home-shot-rail-top{display:grid;grid-template-columns:minmax(0,1fr) auto;align-items:center;gap:6px}.home-shot-rail-actions{display:flex;align-items:center;justify-content:center}.home-shot-rail-actions .shot-panel-order-handle{width:34px;min-width:34px;height:40px}@media (min-width:981px){.shot-profile-board{grid-template-columns:minmax(0,.64fr) minmax(255px,.36fr);grid-template-areas:"chart rail"}.shot-profile-board.profiles-first{grid-template-columns:minmax(255px,.36fr) minmax(0,.64fr);grid-template-areas:"rail chart"}}@media (max-width:980px){.shot-profile-board{grid-template-areas:"chart""rail"}.shot-profile-board.profiles-first{grid-template-areas:"rail""chart"}}.home-top-grid>[data-home-top-panel]{position:relative;min-width:0;transition:opacity .15s ease,transform .15s ease,outline-color .15s ease}.home-top-panel-order-handle{display:none;position:absolute;top:-11px;left:50%;z-index:14;width:42px;min-width:42px;height:25px;place-items:center;padding:0 10px;border:1px solid #596574;border-radius:999px;background:#262d36;color:#d8dee7;font-size:17px;line-height:1;cursor:grab;box-shadow:0 5px 16px rgba(0,0,0,.35);transform:translateX(-50%);touch-action:none;user-select:none}.home-layout-editing .home-overview>.home-top-grid{pointer-events:auto}.home-layout-editing .home-top-grid>[data-home-top-panel]{margin-top:8px;outline:1px dashed #596574;outline-offset:3px;border-radius:16px}.home-layout-editing .home-top-panel-order-handle{display:grid}.home-layout-editing .home-top-grid>[data-home-top-panel]>:not(.home-top-panel-order-handle){pointer-events:none}.home-top-panel-order-handle:active{cursor:grabbing;transform:translateX(-50%) scale(.96)}.home-top-panel-dragging{opacity:.4;transform:scale(.992);outline-color:#e18b48!important}@media (min-width:841px){.home-top-grid.metrics-first{grid-template-columns:minmax(330px,2fr) minmax(430px,3fr)}}@media (max-width:840px) and (min-width:701px){.home-top-grid.metrics-first{grid-template-columns:minmax(290px,2fr) minmax(350px,3fr)}}.home-layout-editing .home-top-panel-order-handle,body.home-top-panel-layout-dragging{cursor:ew-resize}#page-home.home-layout-editing #shotProfileBoard>[data-shot-panel],#page-home.home-layout-editing .home-top-grid>[data-home-top-panel]{pointer-events:auto!important}#page-home.home-layout-editing .home-top-panel-order-handle{display:grid!important;pointer-events:auto!important;visibility:visible!important;opacity:1!important;z-index:60;cursor:ew-resize!important;cursor:grab!important}#page-home.home-layout-editing .home-top-panel-order-handle:active,body.home-top-panel-layout-dragging{cursor:grabbing!important}@media (min-width:701px){.home-top-grid,.home-top-grid.metrics-first{gap:0;grid-template-columns:minmax(0,var(--home-top-machine-share,56fr)) 12px minmax(0,var(--home-top-metrics-share,44fr));grid-template-areas:"machine resize metrics"}.home-top-grid.metrics-first{grid-template-columns:minmax(0,var(--home-top-metrics-share,44fr)) 12px minmax(0,var(--home-top-machine-share,56fr));grid-template-areas:"metrics resize machine"}.home-top-grid>[data-home-top-panel=machine]{grid-area:machine}.home-top-grid>[data-home-top-panel=metrics]{grid-area:metrics}.home-top-resize-handle{grid-area:resize}}.home-top-resize-handle{position:relative;z-index:45;width:12px;min-width:12px;height:100%;min-height:72px;display:none;align-items:center;justify-content:center;align-self:stretch;padding:0;border:0;border-radius:0;background:0 0;box-shadow:none;touch-action:none}.home-top-resize-handle>span,.shot-profile-resize-handle>span{width:4px;height:48px;border-radius:999px;background:#4a5562;box-shadow:0 0 0 1px rgba(0,0,0,.22),0 3px 10px rgba(0,0,0,.22);transition:height .15s ease,background .15s ease}.home-top-resize-handle:hover>span,body.home-top-resizing .home-top-resize-handle>span{height:64px;background:#d28a50}.home-top-resize-handle output,.shot-profile-resize-handle output{position:absolute;top:50%;left:50%;min-width:54px;padding:4px 6px;border:1px solid #4b5664;border-radius:8px;background:#20262e;color:#dce2e9;font-size:9px;font-weight:700;line-height:1;opacity:0;pointer-events:none;transform:translate(-50%,-50%) translateY(-42px);transition:opacity .12s ease;white-space:nowrap}.home-top-resize-handle:hover output,.shot-profile-resize-handle:hover output,body.home-top-resizing .home-top-resize-handle output,body.shot-profile-resizing .shot-profile-resize-handle output{opacity:1}#page-home.home-layout-editing .home-top-resize-handle,#page-home.home-layout-editing .shot-profile-resize-handle{display:flex;pointer-events:auto!important;visibility:visible!important;opacity:1!important}.home-top-resize-handle,body.home-top-resizing,body.shot-profile-resizing{cursor:col-resize;user-select:none}@media (max-width:700px){.home-top-grid,.home-top-grid.metrics-first{grid-template-columns:minmax(0,1fr);grid-template-areas:none;gap:12px}.home-top-grid>[data-home-top-panel]{grid-area:auto}.home-top-resize-handle{display:none!important}}@media (min-width:981px){.shot-profile-board,.shot-profile-board.profiles-first{gap:0}.shot-profile-board{grid-template-columns:minmax(0,var(--shot-chart-share,
              64fr
            )) 12px minmax(255px,var(--shot-profile-share,
              36fr
            ));grid-template-areas:"chart resize rail"}.shot-profile-board.profiles-first{grid-template-columns:minmax(255px,var(--shot-profile-share,
              36fr
            )) 12px minmax(0,var(--shot-chart-share,
              64fr
            ));grid-template-areas:"rail resize chart"}.shot-profile-resize-handle{grid-area:resize}}.shot-profile-resize-handle{position:relative;z-index:45;width:12px;min-width:12px;height:100%;min-height:100px;display:none;align-items:center;justify-content:center;align-self:stretch;padding:0;border:0;border-radius:0;background:0 0;box-shadow:none;cursor:col-resize;touch-action:none;user-select:none}.shot-profile-resize-handle>span{height:54px}.shot-profile-resize-handle:hover>span,body.shot-profile-resizing .shot-profile-resize-handle>span{height:72px;background:#d28a50}.shot-profile-resize-handle output{transform:translate(-50%,-50%) translateY(-45px)}#page-home.home-layout-editing .shot-profile-resize-handle{display:flex!important}@media (max-width:980px){.shot-profile-resize-handle{display:none!important}}#page-home .shot-panel-order-handle{display:none}#page-home.home-layout-editing .shot-panel-order-handle{display:grid!important;pointer-events:auto!important;visibility:visible!important;opacity:1!important}.chart-line-settings-button{width:34px;min-width:34px;height:34px;padding:0;display:grid;place-items:center;font-size:17px;line-height:1}.chart-line-settings-button.active{border-color:#5e7187;background:#25303b;color:#dce8f5}.shot-line-quick-panel{margin-top:6px;padding:8px;border:1px solid #303945;border-radius:10px;background:#0e1318}.shot-line-quick-panel[hidden]{display:none!important}.shot-line-quick-head{display:flex;align-items:center;justify-content:space-between;gap:10px;margin-bottom:6px}.shot-line-quick-head>div:first-child{min-width:0}.shot-line-quick-head b,.shot-line-quick-head small{display:block}.shot-line-quick-head b{color:#dbe1e8;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium)}.shot-line-quick-head small{margin-top:1px;color:#7f8a97;font-size:var(--zone-font-chart)!important}.shot-line-quick-actions{display:flex;align-items:center;justify-content:flex-end;gap:5px;flex:0 0 auto}.shot-line-quick-actions button{min-height:30px;padding:5px 9px;white-space:nowrap}.shot-line-quick-apply{border-color:#35694f!important;background:#183426!important;color:#a8e7c2!important}.shot-line-quick-grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(300px,1fr));gap:5px}.shot-line-quick-row{display:grid;grid-template-columns:minmax(68px,.48fr) 42px minmax(92px,1fr) minmax(92px,1fr);align-items:center;gap:6px;min-height:48px;padding:5px 7px;border:1px solid #28313b;border-radius:9px;background:#11171c}.shot-line-quick-control output,.shot-line-quick-row>b{font-weight:var(--ha-font-weight-medium);white-space:nowrap}.shot-line-quick-row>b{min-width:0;overflow:hidden;color:#cbd3dc;font-size:var(--zone-font-text)!important;text-overflow:ellipsis}.shot-line-color-control,.shot-line-quick-control{min-width:0}.shot-line-color-control>span,.shot-line-quick-control>span{display:flex;align-items:baseline;gap:5px;color:#8f9aa6;font-size:var(--zone-font-chart)!important;line-height:1.15}.shot-line-quick-control>span{justify-content:space-between}.shot-line-color-control>span{justify-content:center}.shot-line-color-control input[type=color]{width:34px;height:23px;display:block;margin:3px auto 0;padding:1px;border:1px solid #3a4652;border-radius:6px;background:#171d24;cursor:pointer}.shot-line-quick-control output{color:#dce2e8;font-size:var(--zone-font-chart)!important}.shot-line-quick-control input[type=range]{width:100%;height:14px;margin:3px 0 0;accent-color:#728296}@media (min-width:981px){.shot-profile-board.line-settings-open{align-items:start}.shot-profile-board.line-settings-open>#shotChartCard{align-self:start;height:auto}.shot-profile-board.line-settings-open>#profileLibraryCard{align-self:start;height:var(--line-settings-profile-height,auto)!important;min-height:var(--line-settings-profile-height,0px)!important;max-height:var(--line-settings-profile-height,none)!important}}@media (max-width:760px){.shot-line-quick-head{align-items:flex-start}.shot-line-quick-grid{grid-template-columns:minmax(0,1fr)}}@media (max-width:520px){.shot-line-quick-head{display:block}.shot-line-quick-actions{justify-content:flex-start;margin-top:6px}.shot-line-quick-row{grid-template-columns:minmax(68px,.48fr) 42px minmax(0,1fr)}.shot-line-quick-control:last-child{grid-column:3}}.machine-control-card{min-height:0;height:100%;display:grid;padding:0!important;overflow:hidden;border:0!important;background:0 0!important;box-shadow:none!important}.machine-status-banner{display:grid;grid-template-rows:auto auto;align-content:center;gap:5px;overflow:hidden;border:1px solid #303945;border-radius:13px;background:linear-gradient(145deg,#181e25,#12171c);box-shadow:inset 0 1px 0 rgba(255,255,255,.035),0 4px 12px rgba(0,0,0,.16)}.machine-status-banner .machine-control-head{min-width:0;gap:8px}.machine-status-banner .machine-control-primary{gap:5px 7px}.machine-status-banner .machine-control-title{font-size:15px}.machine-status-banner .machine-control-primary .machine-state{min-height:21px;padding:3px 7px}.machine-status-banner .machine-control-head-actions{gap:6px}.machine-status-banner .machine-secondary-row{min-height:18px;margin:0;padding:0 1px}.machine-status-banner .machine-auto-off,.machine-status-banner .machine-mode-status{min-height:20px;padding:3px 7px}.machine-control-divider{display:none!important}.machine-actions-dock{min-height:0;display:flex}.machine-actions-dock .machine-quick-actions{width:100%;min-height:0;display:grid;grid-template-columns:repeat(3,minmax(0,1fr));grid-auto-rows:minmax(0,1fr);margin:0}.machine-actions-dock .machine-quick-actions .quick-action{width:100%;grid-template-rows:1fr;align-items:center;align-content:center;justify-items:start;border:1px solid #3b4653!important;background:linear-gradient(145deg,#2b333d,#21272f);color:#edf1f5}.machine-actions-dock .machine-quick-actions .quick-action:hover:not(:disabled):not(.active):not(.danger){background:linear-gradient(145deg,#353e49,#29313a);transform:translateY(-1px);background:linear-gradient(145deg,#333c47,#282f38);border-color:#5c6978!important;background:linear-gradient(145deg,#343d48,#282f38)}.machine-actions-dock .machine-quick-actions .quick-icon{background:#3a444f}.machine-actions-dock .machine-quick-actions .quick-action>span:last-child{min-width:0;display:block;width:100%;display:grid;place-items:center}.machine-actions-dock .machine-quick-actions .quick-action b{margin:0}.machine-actions-dock .machine-quick-actions .quick-action small{width:100%;margin-top:4px;color:#99a4b1;font-size:9px;line-height:1.15;text-align:left}.machine-actions-dock .machine-quick-actions .quick-action:disabled{opacity:1;color:#75808d}.machine-actions-dock .machine-quick-actions .quick-action:disabled .quick-icon{background:#2a3139}.machine-actions-dock .machine-quick-actions .quick-action:disabled b{color:#7b8692}.machine-actions-dock .machine-quick-actions .quick-action:disabled small{color:#626d79}.machine-actions-dock .machine-quick-actions .quick-action.active,.machine-actions-dock .machine-quick-actions .quick-action.danger,.machine-actions-dock .machine-quick-actions .quick-action.steam.active,.machine-actions-dock .machine-quick-actions .quick-action.water.active{border-color:transparent!important}@media (max-width:900px) and (min-width:701px){.machine-control-card{grid-template-rows:minmax(58px,.72fr) minmax(70px,1fr)}.machine-actions-dock .machine-quick-actions .quick-action{grid-template-columns:minmax(0,1fr);justify-items:center;gap:5px;padding:7px 5px;text-align:center}.machine-actions-dock .machine-quick-actions .quick-icon{width:34px;height:34px;margin:0 auto}.machine-actions-dock .machine-quick-actions .quick-icon svg{width:20px;height:20px}.machine-actions-dock .machine-quick-actions .quick-action b,.machine-actions-dock .machine-quick-actions .quick-action small{text-align:center}.machine-actions-dock .machine-quick-actions .quick-action small{display:none}}@media (max-width:700px){.machine-control-card{min-height:148px;grid-template-rows:minmax(58px,.72fr) minmax(76px,1fr)}}@media (max-width:430px){.machine-actions-dock .machine-quick-actions{gap:6px}.machine-actions-dock .machine-quick-actions .quick-action{grid-template-columns:minmax(0,1fr);justify-items:center;gap:4px;padding:6px 4px;text-align:center}.machine-actions-dock .machine-quick-actions .quick-icon{width:33px;height:33px;margin:0 auto}.machine-actions-dock .machine-quick-actions .quick-icon svg{width:19px;height:19px}.machine-actions-dock .machine-quick-actions .quick-action b{font-size:12px;text-align:center}.machine-actions-dock .machine-quick-actions .quick-action small{display:none}}.machine-actions-dock .machine-quick-actions .quick-action{grid-template-columns:minmax(0,1fr);grid-template-rows:minmax(0,1fr);gap:0;border-color:#394552!important;background:linear-gradient(145deg,#29313a,#20262e);box-shadow:inset 0 1px 0 rgba(255,255,255,.045),0 4px 11px rgba(0,0,0,.18)}.machine-actions-dock .machine-quick-actions .quick-action::before{content:"";position:absolute;top:7px;left:50%;width:20px;height:3px;border-radius:999px;background:#56616e;opacity:.58;transform:translateX(-50%);transition:width .18s ease,opacity .18s ease,background .18s ease,box-shadow .18s ease;display:none!important}.machine-actions-dock .machine-quick-actions .quick-action small,.machine-actions-dock .machine-quick-actions .quick-icon{display:none!important}.machine-actions-dock .machine-quick-actions .quick-action b{width:100%;color:#eef2f6;font-size:16px;line-height:1;font-weight:780;text-align:center;letter-spacing:.005em}.machine-actions-dock .machine-quick-actions .quick-action:hover:not(:disabled):not(.active):not(.danger)::before{width:28px;opacity:.9}.machine-actions-dock .machine-quick-actions #shotAction.danger{border-color:#8b4741!important;background:linear-gradient(145deg,#5a2c2a,#371f20);color:#ffd5ce;box-shadow:inset 0 1px 0 rgba(255,255,255,.065),0 5px 15px rgba(101,39,34,.3)}.machine-actions-dock .machine-quick-actions #shotAction.danger::before{width:34px;background:#ff856f;opacity:1;box-shadow:0 0 10px rgba(255,117,92,.44)}.machine-actions-dock .machine-quick-actions #shotAction.danger b{color:#ffe1db}.machine-actions-dock .machine-quick-actions #waterAction.active{border-color:#376f98!important;background:linear-gradient(145deg,#204d6a,#183246);box-shadow:inset 0 1px 0 rgba(255,255,255,.065),0 5px 15px rgba(28,92,134,.28)}.machine-actions-dock .machine-quick-actions #waterAction.active::before{width:34px;background:#6fc7ff;opacity:1;box-shadow:0 0 10px rgba(88,188,255,.42)}.machine-actions-dock .machine-quick-actions #waterAction.active b{color:#dff3ff}.machine-actions-dock .machine-quick-actions #steamAction.active{border-color:#8b6632!important;background:linear-gradient(145deg,#5a421e,#352817);box-shadow:inset 0 1px 0 rgba(255,255,255,.065),0 5px 15px rgba(116,77,25,.28)}.machine-actions-dock .machine-quick-actions #steamAction.active::before{width:34px;background:#ffd166;opacity:1;box-shadow:0 0 10px rgba(255,209,102,.42)}.machine-actions-dock .machine-quick-actions #steamAction.active b{color:#fff0c4}.machine-actions-dock .machine-quick-actions .quick-action:disabled{border-color:#2f3944!important;background:linear-gradient(145deg,#20262d,#191e24);box-shadow:inset 0 1px 0 rgba(255,255,255,.022),0 3px 9px rgba(0,0,0,.13)}.machine-actions-dock .machine-quick-actions .quick-action:disabled::before{background:#3a444f;opacity:.35}@media (max-width:900px) and (min-width:701px){.machine-actions-dock .machine-quick-actions .quick-action{min-height:56px;padding:7px 5px}.machine-actions-dock .machine-quick-actions .quick-action b{font-size:14px}}@media (max-width:700px){.machine-control-card{min-height:136px;grid-template-rows:minmax(58px,.82fr) minmax(64px,.82fr)}}@media (max-width:430px){.machine-actions-dock .machine-quick-actions{gap:6px}.machine-actions-dock .machine-quick-actions .quick-action{min-height:54px;padding:7px 4px}.machine-actions-dock .machine-quick-actions .quick-action b{font-size:13px}}.machine-actions-dock,.machine-control-card,.machine-panel,.machine-status-banner,.machine-top-strip{width:100%;min-width:0}.machine-top-strip{display:grid}.machine-power-tile{display:grid;place-items:center;border:1px solid #303945;border-radius:13px;background:linear-gradient(145deg,#181e25,#12171c);box-shadow:inset 0 1px 0 rgba(255,255,255,.035),0 4px 12px rgba(0,0,0,.16)}.machine-power-tile .power-inline{display:grid;place-items:center;padding:0;border-radius:50%;border:1px solid transparent;box-shadow:inset 0 1px 0 rgba(255,255,255,.08),0 5px 14px rgba(0,0,0,.24);transition:background .18s ease,border-color .18s ease,color .18s ease,box-shadow .18s ease,transform .12s ease}.machine-power-tile .power-inline:hover{transform:translateY(-1px)}.machine-power-tile .power-inline.off{border-color:#71363b;background:linear-gradient(145deg,#54272d,#351b20);color:#ff6e6e;box-shadow:inset 0 1px 0 rgba(255,255,255,.08),0 5px 14px rgba(106,38,45,.28)}.machine-power-tile .power-inline.on{border-color:#34704f;background:linear-gradient(145deg,#24563d,#173524);color:#7be5a5;box-shadow:inset 0 1px 0 rgba(255,255,255,.08),0 5px 14px rgba(31,105,66,.28)}.machine-power-tile .power-symbol{font-size:23px;line-height:1}.machine-power-label{position:absolute!important;width:1px!important;height:1px!important;overflow:hidden!important;clip:rect(0 0 0 0)!important;clip-path:inset(50%)!important;white-space:nowrap!important}.machine-status-banner{padding:8px 10px}.machine-actions-dock .machine-quick-actions{height:100%;gap:7px}.machine-actions-dock .machine-quick-actions .quick-action{height:100%;display:grid;place-items:center;border-radius:13px;text-align:center}.machine-actions-dock .machine-quick-actions .quick-icon{color:#aab4c0}.machine-actions-dock .machine-quick-actions .quick-icon svg{fill:none;stroke:currentColor;stroke-width:1.75;stroke-linecap:round;stroke-linejoin:round}.machine-actions-dock .machine-action-copy{position:absolute!important;width:1px!important;height:1px!important;overflow:hidden!important;clip:rect(0 0 0 0)!important;clip-path:inset(50%)!important;white-space:nowrap!important}.machine-actions-dock .machine-quick-actions .quick-action:not(:disabled):not(.active):not(.danger){border-color:#394552!important;background:linear-gradient(145deg,#29313a,#20262e)}.machine-actions-dock .machine-quick-actions #shotAction.danger .quick-icon{color:#ff8a75}.machine-actions-dock .machine-quick-actions #waterAction.active .quick-icon{color:#77caff}.machine-actions-dock .machine-quick-actions #steamAction.active .quick-icon{color:#ffd166}.machine-actions-dock .machine-quick-actions .quick-action:disabled .quick-icon{color:#5f6975}@media (min-width:701px){.home-top-grid{width:100%;min-width:0;grid-template-columns:minmax(0,56fr) 12px minmax(0,44fr)}.home-top-grid.metrics-first{width:100%;min-width:0;grid-template-columns:minmax(0,44fr) 12px minmax(0,56fr)}}@media (max-width:900px) and (min-width:701px){.machine-top-strip{grid-template-columns:58px minmax(0,1fr)}.machine-power-tile .power-inline{width:41px;min-width:41px;height:41px}.machine-actions-dock .machine-quick-actions .quick-icon{width:36px;height:36px}.machine-actions-dock .machine-quick-actions .quick-icon svg{width:28px;height:28px}}@media (max-width:700px){.machine-control-card{min-height:134px;grid-template-rows:minmax(62px,.86fr) minmax(58px,.78fr)}}@media (max-width:430px){.machine-top-strip{grid-template-columns:54px minmax(0,1fr);gap:6px}.machine-power-tile{padding:6px}.machine-power-tile .power-inline{width:39px;min-width:39px;height:39px}.machine-actions-dock .machine-quick-actions{gap:6px}.machine-actions-dock .machine-quick-actions .quick-icon{width:34px;height:34px}.machine-actions-dock .machine-quick-actions .quick-icon svg{width:26px;height:26px}}@media (min-width:701px){.home-top-grid,.home-top-grid.metrics-first{width:100%;min-width:0;justify-content:stretch;grid-template-columns:minmax(0,var(--home-top-machine-track,
              calc(56% - 6px)
            )) 12px minmax(0,var(--home-top-metrics-track,
              calc(44% - 6px)
            ))!important}.home-top-grid.metrics-first{grid-template-columns:minmax(0,var(--home-top-metrics-track,
              calc(44% - 6px)
            )) 12px minmax(0,var(--home-top-machine-track,
              calc(56% - 6px)
            ))!important}}.machine-control-card{grid-template-rows:78px minmax(58px,1fr);gap:7px}.machine-top-strip{height:78px;min-height:78px;grid-template-columns:78px minmax(0,1fr);gap:7px;align-items:stretch}.machine-power-tile{width:78px;min-width:78px;aspect-ratio:1;place-self:start;padding:12px}.machine-power-tile .power-inline{width:52px;min-width:52px;height:52px;min-height:52px;margin:0;place-self:center;box-sizing:border-box}.machine-power-tile,.machine-status-banner{min-height:78px;height:78px;box-sizing:border-box}.machine-actions-dock .machine-quick-actions .quick-action{position:relative;min-height:58px;overflow:hidden;display:block!important;padding:0!important}.machine-actions-dock .machine-quick-actions .quick-icon{position:absolute!important;inset:0!important;width:42px!important;height:42px!important;margin:auto!important;display:grid!important;place-items:center!important;align-self:auto!important;justify-self:auto!important;transform:none!important;border-radius:0!important;background:0 0!important}.machine-actions-dock .machine-quick-actions .quick-icon svg{display:block;width:32px!important;height:32px!important;margin:auto;overflow:visible}.machine-actions-dock .machine-quick-actions .quick-action.active .quick-icon,.machine-actions-dock .machine-quick-actions .quick-action.danger .quick-icon,.machine-actions-dock .machine-quick-actions .quick-action:disabled .quick-icon{background:0 0!important;transform:none!important}@media (max-width:900px) and (min-width:701px){.machine-control-card{grid-template-rows:70px minmax(56px,1fr)}.machine-top-strip{height:70px;min-height:70px;grid-template-columns:70px minmax(0,1fr)}.machine-power-tile{width:70px;min-width:70px;padding:11px}.machine-power-tile .power-inline{width:46px;min-width:46px;height:46px;min-height:46px}.machine-power-tile,.machine-status-banner{min-height:70px;height:70px}.machine-actions-dock .machine-quick-actions .quick-icon{width:38px!important;height:38px!important}.machine-actions-dock .machine-quick-actions .quick-icon svg{width:29px!important;height:29px!important}}@media (max-width:700px){.machine-control-card{min-height:138px;grid-template-rows:68px minmax(58px,1fr)}.machine-top-strip{height:68px;min-height:68px;grid-template-columns:68px minmax(0,1fr)}.machine-power-tile{width:68px;min-width:68px;padding:10px}.machine-power-tile .power-inline{width:46px;min-width:46px;height:46px;min-height:46px}.machine-power-tile,.machine-status-banner{min-height:68px;height:68px}}@media (max-width:430px){.machine-control-card{grid-template-rows:62px minmax(54px,1fr)}.machine-top-strip{height:62px;min-height:62px;grid-template-columns:62px minmax(0,1fr);gap:6px}.machine-power-tile{width:62px;min-width:62px;padding:9px}.machine-power-tile .power-inline{width:42px;min-width:42px;height:42px;min-height:42px}.machine-power-tile,.machine-status-banner{min-height:62px;height:62px}.machine-actions-dock .machine-quick-actions .quick-icon{width:35px!important;height:35px!important}.machine-actions-dock .machine-quick-actions .quick-icon svg{width:27px!important;height:27px!important}}.machine-icon-size-tools{flex:1 0 100%;display:grid;align-items:end;gap:8px 10px;padding:9px 10px;border:1px solid #303a46;border-radius:10px;background:#11161b}.machine-icon-size-tools label{min-width:0;display:grid;grid-template-columns:minmax(0,1fr) auto;gap:5px 9px;align-items:center;color:#aeb8c4;font-size:10px;font-weight:700}.machine-icon-size-tools output{color:#e5eaf0;font-variant-numeric:tabular-nums}.machine-icon-size-tools input{grid-column:1/-1;width:100%;min-width:0;height:18px;padding:0;border:0;background:0 0}.machine-icon-size-tools>button{align-self:end;white-space:nowrap}#page-home .home-top-grid>[data-home-top-panel]{width:100%!important;max-width:none!important;min-width:0!important;justify-self:stretch!important;align-self:stretch!important;box-sizing:border-box}#page-home .home-top-grid>.machine-panel{display:grid!important;grid-template-columns:minmax(0,1fr)}#page-home .home-top-grid>.home-metrics-panel,#page-home .machine-panel>.machine-control-card{width:100%!important;max-width:none!important;min-width:0!important;justify-self:stretch!important;align-self:stretch!important;box-sizing:border-box}#page-home{--machine-action-icon-size:48px;--machine-power-button-size:52px;--home-section-gap:18px;--home-temperature-card-height:165px;--home-left-column-share:50fr;--home-right-column-share:50fr;--home-left-column-share:38fr;--home-pressure-column-share:18fr;--home-right-column-share:44fr}#page-home .machine-power-tile .power-inline{width:var(--machine-power-button-size)!important;min-width:var(--machine-power-button-size)!important;height:var(--machine-power-button-size)!important;min-height:var(--machine-power-button-size)!important}#page-home .machine-power-tile .power-symbol{font-size:calc(var(--machine-power-button-size)*.44)!important}#page-home .machine-actions-dock .machine-quick-actions .quick-icon svg{display:block!important;margin:0!important;transform:none!important;width:min(var(--machine-action-icon-size),42px)!important;height:min(var(--machine-action-icon-size),42px)!important;width:var(--machine-action-icon-size)!important;height:var(--machine-action-icon-size)!important}#page-home .machine-actions-dock .machine-quick-actions .quick-icon{width:calc(var(--machine-action-icon-size) + 10px)!important;height:calc(var(--machine-action-icon-size) + 10px)!important}@media (max-width:760px){.machine-icon-size-tools{grid-template-columns:minmax(0,1fr)}.machine-icon-size-tools>button{width:100%}}#page-home .home-top-grid>.machine-panel{position:relative!important;display:block!important;width:100%!important;min-width:0!important;max-width:none!important;overflow:visible}#page-home .home-top-grid>.machine-panel>.machine-control-card{position:absolute!important;inset:0!important;width:auto!important;min-width:0!important;max-width:none!important;margin:0!important;box-sizing:border-box!important}#page-home .home-top-grid>.machine-panel>.dashboard-drag-handle,#page-home .home-top-grid>.machine-panel>.dashboard-hide-button,#page-home .home-top-grid>.machine-panel>.home-top-panel-order-handle{z-index:80}@media (max-width:700px){#page-home .home-top-grid>.machine-panel{min-height:138px}}@media (max-width:430px){#page-home .home-top-grid>.machine-panel{min-height:126px}}#page-home .machine-control-card{grid-template-rows:78px 80px!important;min-height:165px!important}#page-home .machine-actions-dock .machine-quick-actions{display:grid;grid-auto-columns:123px;align-content:center;align-items:center}#page-home .machine-actions-dock .machine-quick-actions .quick-action{box-sizing:border-box}@media (max-width:700px){#page-home .machine-control-card{grid-template-rows:68px 80px!important;min-height:155px!important}#page-home .home-top-grid>.machine-panel{min-height:155px!important}}@media (max-width:430px){#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,1fr))!important;grid-auto-columns:auto;width:100%}#page-home .machine-actions-dock .machine-quick-actions .quick-action{width:100%!important;min-width:0!important;max-width:none!important}}#page-home .machine-control-card{overflow:visible!important;border-radius:0!important;clip-path:none!important}#page-home .machine-power-tile{position:relative;isolation:isolate;overflow:visible}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{position:relative!important;display:block!important;margin:0!important;padding:0!important;border-width:1px!important;border-style:solid!important;box-sizing:border-box!important;transform:none!important;translate:none!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action.active .quick-icon,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger .quick-icon,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active .quick-icon,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled .quick-icon,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover .quick-icon,#page-home .machine-actions-dock .machine-quick-actions .quick-icon{position:absolute!important;top:50%!important;left:50%!important;right:auto!important;bottom:auto!important;width:calc(var(--machine-action-icon-size) + 10px)!important;height:calc(var(--machine-action-icon-size) + 10px)!important;margin:0!important;padding:0!important;display:grid!important;place-items:center!important;transform:translate(-50%,-50%)!important;translate:none!important;background:0 0!important;border:0!important;border-radius:0!important;box-shadow:none!important}#page-home .machine-status-banner{display:grid!important;grid-template-rows:minmax(0,1fr)}#page-home .machine-status-main{min-width:0;display:flex;flex-direction:column;align-items:flex-start;justify-content:center;gap:5px;padding:0 4px}#page-home .machine-status-main .machine-mode-status{min-width:0;min-height:30px;display:inline-flex;align-items:center;gap:7px;margin:0;padding:6px 10px;border:1px solid #34404c;border-radius:10px;background:#171d24}#page-home .machine-status-main .machine-mode-status b{overflow:hidden;color:#e8edf3;font-size:13px;line-height:1;text-overflow:ellipsis;white-space:nowrap}#page-home .machine-status-main .machine-auto-off{min-height:20px;margin:0;padding:3px 7px}#page-home .machine-water-tile{position:relative;display:grid!important;place-items:center;padding:0!important;overflow:hidden;border:1px solid #35404c!important;border-radius:11px!important;background:#192027!important;box-shadow:inset 0 1px 0 rgba(255,255,255,.035)}#page-home .machine-water-drop{display:grid;place-items:center;width:28px;height:28px;color:#788492}#page-home .machine-water-drop svg{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}#page-home .machine-water-result{position:absolute;right:3px;bottom:3px;min-width:15px;height:15px;display:grid;place-items:center;padding:0 3px;border:1px solid #44505d;border-radius:999px;background:#252d36;color:#aeb8c4;font-size:9px;line-height:1;font-weight:850}#page-home .machine-water-tile.ok{border-color:#35694c!important;background:linear-gradient(145deg,#183326,#14251d)!important}#page-home .machine-water-tile.ok .machine-water-drop{color:#67d99a}#page-home .machine-water-tile.ok .machine-water-result{border-color:#3d7a58;background:#235139;color:#9af0bd}#page-home .machine-water-tile.warn{border-color:#804347!important;background:linear-gradient(145deg,#3b2327,#281b1e)!important}#page-home .machine-water-tile.warn .machine-water-drop{color:#ff8188}#page-home .machine-water-tile.warn .machine-water-result{border-color:#8d4a50;background:#582b31;color:#ffc0c4}#page-home .machine-status-hidden,#page-home .machine-water-label{position:absolute!important;width:1px!important;height:1px!important;overflow:hidden!important;clip:rect(0 0 0 0)!important;clip-path:inset(50%)!important;white-space:nowrap!important}@media (max-width:430px){#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:100%!important;min-width:0!important;max-width:none!important}#page-home .machine-status-banner{grid-template-columns:minmax(0,1fr) 43px;gap:6px;padding:6px!important}#page-home .machine-water-tile{width:43px!important;min-width:43px!important}}#page-home .machine-water-tile{width:78px!important;min-width:78px!important;height:78px!important;min-height:78px!important;align-self:stretch;box-sizing:border-box}#page-home .machine-status-banner{grid-template-columns:minmax(0,1fr)!important}#page-home .machine-actions-dock .machine-quick-actions{width:100%;grid-template-columns:repeat(4,minmax(0,123px))!important}#page-home .machine-custom-action.empty{border-style:dashed!important;border-color:#4c5865!important;background:linear-gradient(145deg,#222930,#1a2026)!important}#page-home .machine-custom-action.empty .quick-icon{color:#8793a0}#page-home .machine-custom-action:not(.empty){border-color:#5a506e!important;background:linear-gradient(145deg,#342e43,#252131)!important}#page-home .machine-custom-action:not(.empty) .quick-icon{color:#c9b5f0}.machine-shortcut-dialog[hidden]{display:none}.machine-shortcut-dialog{position:fixed;inset:0;z-index:300;display:grid;place-items:center;padding:18px;background:rgba(5,8,11,.72);backdrop-filter:blur(4px)}.machine-shortcut-dialog-card{width:min(390px,100%);padding:16px;border:1px solid #3b4653;border-radius:16px;background:linear-gradient(145deg,#20262e,#171c22);box-shadow:0 20px 55px rgba(0,0,0,.46)}.machine-shortcut-dialog-head{display:flex;align-items:flex-start;justify-content:space-between;gap:12px}.machine-shortcut-dialog-head b{display:block;font-size:16px}.machine-shortcut-dialog-head small{display:block;margin-top:4px;color:#919ca9;font-size:11px}.machine-shortcut-close{width:30px;min-width:30px;height:30px;padding:0;border-radius:9px;background:#2a313a;color:#c8d0da;font-size:18px}.machine-shortcut-field{display:grid;gap:6px;margin-top:15px;color:#aab4c0;font-size:11px;font-weight:700}.machine-shortcut-dialog-actions{display:grid;grid-template-columns:minmax(0,1fr) minmax(0,1fr);gap:8px;margin-top:14px}@media (max-width:900px) and (min-width:701px){#page-home .machine-top-strip{grid-template-columns:70px minmax(0,1fr) 70px!important}#page-home .machine-water-tile{width:70px!important;min-width:70px!important;height:70px!important;min-height:70px!important}}@media (max-width:700px){#page-home .machine-top-strip{grid-template-columns:68px minmax(0,1fr) 68px!important}#page-home .machine-water-tile{width:68px!important;min-width:68px!important;height:68px!important;min-height:68px!important}}@media (max-width:430px){#page-home .machine-top-strip{grid-template-columns:62px minmax(0,1fr) 62px!important}#page-home .machine-water-tile{width:62px!important;min-width:62px!important;height:62px!important;min-height:62px!important}#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(4,minmax(0,1fr))!important;gap:5px}}#page-home .machine-top-strip{grid-template-columns:78px minmax(0,1fr)!important;gap:7px}#page-home .machine-status-banner{position:relative;display:flex!important;align-items:center;justify-content:space-between;gap:12px;padding:11px 13px!important;overflow:hidden}#page-home .machine-status-copy{min-width:0;display:flex;flex-direction:column;align-items:flex-start;justify-content:center;gap:7px}#page-home .machine-status-copy>#shotStatus{display:block;max-width:100%;margin:0;padding:0;border:0!important;border-radius:0!important;background:0 0!important;font-size:18px;line-height:1.05;font-weight:760;overflow:hidden;text-overflow:ellipsis;white-space:nowrap;box-shadow:none!important}#page-home .machine-status-copy .machine-auto-off{min-height:0;margin:0;padding:0;border:0;background:0 0;color:#8f9aa7}#page-home .machine-water-indicator{position:relative;flex:0 0 auto;width:38px!important;min-width:38px!important;height:38px!important;min-height:38px!important;display:grid!important;place-items:center;padding:0!important;border:1px solid #35404c!important;border-radius:11px!important;background:#1c232b!important;box-shadow:inset 0 1px 0 rgba(255,255,255,.035)}#page-home .machine-water-indicator .machine-water-drop{width:24px;height:24px;color:#74808d}#page-home .machine-water-indicator .machine-water-drop svg{width:22px;height:22px}#page-home .machine-water-indicator .machine-water-result{display:none!important}#page-home .machine-water-indicator.ok{border-color:#35694c!important;background:#193126!important}#page-home .machine-water-indicator.ok .machine-water-drop{color:#63d493}#page-home .machine-water-indicator.warn{border-color:#774047!important;background:#352126!important}#page-home .machine-water-indicator.warn .machine-water-drop{color:#f27a82}#page-home .machine-water-indicator.unknown{border-color:#35404c!important;background:#1c232b!important}#page-home .machine-water-indicator.unknown .machine-water-drop{color:#74808d}#page-home .machine-actions-dock .machine-quick-actions .quick-action{width:123px!important;min-width:123px!important;max-width:123px!important;height:80px!important;min-height:80px!important;max-height:80px!important}@media (max-width:900px) and (min-width:701px){#page-home .machine-top-strip{grid-template-columns:70px minmax(0,1fr)!important}}@media (max-width:700px){#page-home .machine-top-strip{grid-template-columns:68px minmax(0,1fr)!important}#page-home .machine-status-copy>#shotStatus{font-size:16px}}@media (max-width:430px){#page-home .machine-top-strip{grid-template-columns:62px minmax(0,1fr)!important}#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,1fr))!important;gap:6px}#page-home .machine-actions-dock .machine-quick-actions .quick-action{width:100%!important;min-width:0!important;max-width:none!important}#page-home .machine-water-indicator{width:34px!important;min-width:34px!important;height:34px!important;min-height:34px!important}}#page-home .home-metrics-panel{min-height:165px;height:100%;padding:0!important;border:0!important;border-radius:0!important;background:0 0!important;box-shadow:none!important;overflow:visible}#page-home .home-metrics-grid{width:100%;min-width:0;min-height:165px!important;height:165px;display:grid;grid-template-columns:repeat(3,minmax(0,1fr))!important;grid-template-rows:minmax(0,1fr)!important;align-items:stretch;gap:10px}#page-home .home-metrics-grid>.home-metric-card{width:100%;min-width:0;height:165px;min-height:165px;display:flex;flex-direction:column;justify-content:center;box-sizing:border-box}#page-home .home-metrics-grid .home-metric-icon{width:38px;min-width:38px;height:38px;margin:0;border-radius:11px}#page-home .home-metrics-grid .home-metric-icon svg{width:23px;height:23px}#page-home .home-metrics-grid .home-metric-copy{width:100%;min-width:0}#page-home .home-metrics-grid .home-metric-copy small{display:block;overflow:hidden;font-size:10px;line-height:1.1;text-overflow:ellipsis;white-space:nowrap}#page-home .home-metrics-grid .home-metric-copy strong,#page-home .home-metrics-grid .home-metric-copy>span{display:block;margin-top:7px;overflow:hidden;font-size:24px;line-height:1;text-overflow:ellipsis;white-space:nowrap}#page-home .home-metrics-grid .home-metric-copy>span{font-size:9px;line-height:1.15}#page-home .home-metrics-grid .home-metric-progress{left:13px;right:13px;bottom:11px}#page-home .home-metrics-grid .metric-open-indicator{top:9px;right:9px}#page-home .home-metrics-grid .temperature-metric-open .home-metric-state{right:33px}@media (max-width:900px) and (min-width:701px){#page-home .home-metrics-grid,#page-home .home-metrics-panel{min-height:155px!important;height:155px}#page-home .home-metrics-grid>.home-metric-card{height:155px;min-height:155px;padding:13px 10px}#page-home .home-metrics-grid .home-metric-copy strong{font-size:20px}}@media (max-width:700px){#page-home .home-metrics-grid,#page-home .home-metrics-panel{min-height:138px!important;height:138px}#page-home .home-metrics-grid>.home-metric-card{height:138px;min-height:138px;padding:12px 10px}#page-home .home-metrics-grid .home-metric-copy strong{font-size:19px}}@media (max-width:430px){#page-home .home-metrics-grid{grid-template-columns:repeat(3,minmax(0,1fr))!important;grid-template-rows:minmax(0,1fr)!important;gap:6px}#page-home .home-metrics-grid>.home-metric-card{padding:10px 7px}#page-home .home-metrics-grid .home-metric-icon{width:32px;min-width:32px;height:32px}#page-home .home-metrics-grid .home-metric-icon svg{width:20px;height:20px}#page-home .home-metrics-grid .home-metric-copy small,#page-home .home-metrics-grid .home-metric-copy>span{font-size:8px}#page-home .home-metrics-grid .home-metric-copy strong{font-size:16px}}#page-home .home-dashboard-layout{gap:var(--home-section-gap)!important}@media (max-width:980px){.machine-icon-size-tools{grid-template-columns:repeat(2,minmax(210px,1fr))}.machine-icon-size-tools>button{grid-column:1/-1;width:100%}}@media (max-width:620px){.machine-icon-size-tools{grid-template-columns:minmax(0,1fr)}.machine-icon-size-tools>button{grid-column:auto}}#page-home .machine-status-copy>#shotStatus{font-family:var(--ha-font-family-body)!important;font-size:var(--zone-font-title,16px)!important;font-weight:var(--ha-font-weight-medium)!important;line-height:var(--ha-line-height-condensed)!important;letter-spacing:0!important;color:#d8dee7!important;text-rendering:optimizeLegibility;-webkit-font-smoothing:antialiased}@media (max-width:700px){#page-home .machine-status-copy>#shotStatus{font-size:var(--zone-font-title,16px)!important}}@media (min-width:701px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{width:100%;min-width:0;display:grid;grid-template-columns:repeat(2,minmax(0,1fr))!important;grid-template-areas:none!important;gap:12px!important;align-items:stretch}#page-home .home-top-grid>.home-metrics-panel,#page-home .home-top-grid>.machine-panel{grid-area:auto!important;width:100%!important;min-width:0!important;max-width:none!important;align-self:stretch!important;justify-self:stretch!important}}@media (max-width:700px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{grid-template-columns:minmax(0,1fr)!important;grid-template-areas:none!important;gap:12px!important}}#page-home .home-top-panel-order-handle,#page-home .home-top-resize-handle{display:none!important;visibility:hidden!important;pointer-events:none!important}#page-home.home-layout-editing .home-top-grid>.home-metrics-panel,#page-home.home-layout-editing .home-top-grid>.machine-panel{outline:0!important;outline-offset:0!important;box-shadow:none}#page-home.home-layout-editing .home-overview.dashboard-block>.home-top-grid,#page-home.home-layout-editing .home-overview.dashboard-block>.home-top-grid>*{pointer-events:none!important}#page-home .home-metrics-grid>.home-metric-card{position:relative;display:grid;grid-template-rows:38px minmax(0,1fr) 26px;align-items:stretch;gap:7px;padding:13px 13px 12px;overflow:hidden;border:1px solid #303945;border-radius:14px;background:linear-gradient(155deg,#1b222a 0,#151a20 58%,#12171c 100%);box-shadow:inset 0 1px 0 rgba(255,255,255,.035),0 5px 15px rgba(0,0,0,.16)}#page-home .home-metric-head{justify-content:space-between}#page-home .home-metric-head,#page-home .home-metric-title{min-width:0;display:flex;align-items:center;gap:8px}#page-home .home-metric-title>small{overflow:hidden;color:#aeb8c4;font-size:var(--zone-font-small,10px)!important;font-weight:var(--ha-font-weight-medium);line-height:1.1;text-overflow:ellipsis;white-space:nowrap}#page-home .home-metric-title .home-metric-icon{width:34px;min-width:34px;height:34px;margin:0;display:grid;place-items:center;border-radius:10px}#page-home .home-metric-title .home-metric-icon svg{width:21px;height:21px}#page-home .home-metric-footer,#page-home .home-metric-main{min-width:0;display:flex;align-items:center;justify-content:flex-start}#page-home .home-metric-main{padding-left:2px}#page-home .home-metric-main strong{display:flex;align-items:baseline;gap:5px;margin:0;color:#eef2f6;font-size:clamp(27px,2.45vw,36px)!important;font-weight:var(--ha-font-weight-medium)!important;line-height:.95;letter-spacing:-.035em}#page-home .home-metric-main strong i{overflow:hidden;font-style:normal;text-overflow:ellipsis}#page-home .home-metric-main strong em{flex:0 0 auto;color:#8f9aa7;font-size:.42em;font-style:normal;font-weight:var(--ha-font-weight-medium);letter-spacing:0}#page-home .home-metric-footer{gap:7px;overflow:hidden;color:#87929f;font-size:var(--zone-font-small,10px);line-height:1;white-space:nowrap}#page-home .home-metric-footer span,#page-home .home-metric-main strong{min-width:0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}#page-home .home-metric-footer span+span::before{content:"·";margin-right:7px;color:#4f5a66}#page-home .home-metric-footer i{color:#cbd3dc;font-style:normal;font-weight:var(--ha-font-weight-medium)}#page-home .home-backflush-main strong{color:#72dda0;font-size:clamp(25px,2.15vw,32px)!important;letter-spacing:-.02em}#page-home .temperature-metric{border-color:#4d3c31;background:linear-gradient(155deg,#211c18 0,#181713 58%,#131512 100%)}#page-home .pressure-metric{border-color:#2d465b;background:linear-gradient(155deg,#17232d 0,#151b21 58%,#12171c 100%)}#page-home .cleaning-metric{border-color:#334b3e;background:linear-gradient(155deg,#17251d 0,#151c18 58%,#121713 100%)}#page-home .temperature-metric .home-metric-main strong{color:#f2d2b7}#page-home .pressure-metric .home-metric-main strong{color:#b9dcff}@media (max-width:900px){#page-home .home-metrics-grid>.home-metric-card{grid-template-rows:34px minmax(0,1fr) 24px;gap:5px;padding:11px 10px 10px}#page-home .home-metric-title .home-metric-icon{width:30px;min-width:30px;height:30px}#page-home .home-metric-title{gap:6px}#page-home .home-metric-footer{gap:4px}#page-home .home-metric-footer span+span::before{margin-right:4px}}@media (max-width:430px){#page-home .home-metrics-grid>.home-metric-card{grid-template-rows:28px minmax(0,1fr) 20px;padding:8px 7px}#page-home .home-metric-title .home-metric-icon{width:26px;min-width:26px;height:26px;border-radius:8px}#page-home .home-metric-title .home-metric-icon svg{width:17px;height:17px}#page-home .home-metric-main strong{font-size:21px!important}#page-home .home-backflush-main strong{font-size:19px!important}#page-home .home-metric-footer{font-size:7px}#page-home .metric-open-indicator{display:none}}#page-home .home-metrics-grid>.metric-tile{position:relative!important;display:grid!important;grid-template-rows:34px minmax(0,1fr) 22px!important;align-items:stretch!important;gap:7px!important;padding:12px 13px 11px!important;overflow:hidden!important;border:1px solid #34404c!important;border-radius:14px!important;background:linear-gradient(155deg,#1c232b,#151a20 72%)!important;box-shadow:inset 0 1px 0 rgba(255,255,255,.035),0 5px 14px rgba(0,0,0,.15)!important}#page-home .metric-tile::before{position:absolute;top:0;left:18px;right:18px;height:2px;border-radius:0 0 999px 999px;opacity:.75}#page-home .temperature-metric::before{background:#d99054}#page-home .pressure-metric::before{background:#5da9ef}#page-home .cleaning-metric::before{background:#65c892}#page-home .metric-tile-head{display:flex;align-items:center;justify-content:space-between;gap:7px}#page-home .metric-tile-label{min-width:0;display:flex;align-items:center;gap:7px;color:#aeb8c4;font-family:var(--ha-font-family-body);font-size:var(--zone-font-small,10px);font-weight:var(--ha-font-weight-medium);line-height:1;white-space:nowrap}#page-home .metric-tile-label>span:last-child{overflow:visible;text-overflow:clip;white-space:nowrap}#page-home .metric-tile-label .home-metric-icon{width:30px!important;min-width:30px!important;height:30px!important;margin:0!important;display:grid!important;place-items:center!important;border-radius:9px!important}#page-home .metric-tile-label .home-metric-icon svg{width:19px!important;height:19px!important}#page-home .metric-tile-value{display:flex;align-items:center;justify-content:center;gap:5px;padding:0 2px;overflow:visible;color:#edf2f7;font-family:var(--ha-font-family-body);font-size:clamp(30px,2.65vw,38px)!important;font-weight:var(--ha-font-weight-medium);line-height:.95;letter-spacing:-.035em;white-space:nowrap}#page-home .metric-tile-value i{font-style:normal;font-weight:inherit}#page-home .metric-tile-value em{align-self:flex-end;margin-bottom:3px;color:#8e99a6;font-size:.36em;font-style:normal;font-weight:var(--ha-font-weight-medium);letter-spacing:0}#page-home .temperature-metric .metric-tile-value{color:#f1c9aa}#page-home .pressure-metric .metric-tile-value{color:#a8d4ff}#page-home .metric-tile-ratio{gap:7px;color:#dce4ec}#page-home .metric-tile-ratio b{color:#66727f;font-size:.65em;font-weight:var(--ha-font-weight-normal);letter-spacing:0}#page-home .metric-tile-meta{display:flex;align-items:center;justify-content:center;gap:5px;overflow:visible;color:#8f9aa7;font-size:9px!important;font-weight:var(--ha-font-weight-normal);line-height:1;white-space:nowrap}#page-home .metric-tile-meta i{color:#cbd4dd;font-style:normal;font-weight:var(--ha-font-weight-medium)}#page-home .metric-tile-meta b{color:#505b67;font-weight:var(--ha-font-weight-normal)}#page-home .metric-status-badge{flex:0 0 auto;min-width:28px;padding:4px 7px;border:1px solid #35694c;border-radius:999px;background:#183025;color:#72dda0;font-size:9px;font-weight:var(--ha-font-weight-bold);line-height:1;text-align:center;white-space:nowrap}#page-home .cleaning-metric.due .metric-status-badge{border-color:#75502f;background:#332419;color:#ffc27f}#page-home .cleaning-metric.due .metric-tile-value{color:#ffc27f}#page-home .metric-open-indicator{position:static!important;flex:0 0 auto;width:24px!important;min-width:24px!important;height:24px!important;display:grid!important;place-items:center!important;border-radius:8px!important;font-size:10px!important;transform:none!important}#page-home .home-metric-progress{left:13px!important;right:13px!important;bottom:7px!important;height:3px!important;opacity:.68}#page-home .temperature-metric-open .home-metric-state{display:none!important}@media (max-width:900px){#page-home .home-metrics-grid>.metric-tile{grid-template-rows:30px minmax(0,1fr) 20px!important;gap:5px!important;padding:10px 9px 9px!important}#page-home .metric-tile-label{gap:5px;font-size:9px}#page-home .metric-tile-label .home-metric-icon{width:27px!important;min-width:27px!important;height:27px!important}#page-home .metric-tile-value{font-size:clamp(25px,2.7vw,32px)!important}#page-home .metric-tile-meta{font-size:8px!important}}@media (max-width:430px){#page-home .home-metrics-grid>.metric-tile{grid-template-rows:26px minmax(0,1fr) 18px!important;padding:8px 6px!important}#page-home .metric-tile-label>span:last-child{display:none}#page-home .metric-tile-value{font-size:21px!important}#page-home .metric-tile-meta{font-size:7px!important}#page-home .metric-status-badge{padding:3px 5px;font-size:8px}#page-home .metric-open-indicator{display:none!important}}#page-home .home-metrics-grid>.metric-tile{grid-template-columns:minmax(0,1fr)!important;grid-auto-flow:row!important}#page-home .metric-tile-head,#page-home .metric-tile-meta,#page-home .metric-tile-value{position:relative!important;inset:auto!important;grid-column:1!important;grid-row:1!important;width:100%!important;min-width:0!important;box-sizing:border-box}#page-home .metric-tile-meta,#page-home .metric-tile-value{grid-row:2!important}#page-home .metric-tile-meta{grid-row:3!important}#page-home .metric-tile::before{content:none!important;display:none!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action::before{content:""!important;position:absolute!important;top:0!important;left:16px!important;right:16px!important;bottom:auto!important;width:auto!important;height:3px!important;display:block!important;border-radius:0 0 999px 999px!important;opacity:.78!important;transform:none!important;transition:opacity .18s ease,box-shadow .18s ease!important;pointer-events:none}#page-home .machine-actions-dock #shotAction::before{background:#e18b48!important}#page-home .machine-actions-dock #waterAction::before{background:#54a9ff!important}#page-home .machine-actions-dock #steamAction::before{background:#d9b85f!important}#page-home .machine-actions-dock #shotAction.danger::before{background:#ff806b!important;opacity:1!important;box-shadow:0 0 9px rgba(255,112,89,.42)!important}#page-home .machine-actions-dock #waterAction.active::before{background:#68bdff!important;opacity:1!important;box-shadow:0 0 9px rgba(84,169,255,.38)!important}#page-home .machine-actions-dock #steamAction.active::before{background:#ffd166!important;opacity:1!important;box-shadow:0 0 9px rgba(255,209,102,.35)!important}#page-home .machine-actions-dock .quick-action:disabled::before{opacity:.28!important;box-shadow:none!important}#page-home .home-top-grid{align-items:stretch!important}#page-home .machine-compact-status{display:grid!important;grid-template-columns:minmax(0,1fr)!important;grid-template-rows:22px minmax(0,1fr)!important;gap:6px!important}#page-home .machine-compact-head{min-width:0;display:flex;align-items:center;justify-content:space-between;gap:8px}#page-home .machine-compact-head>#shotStatus{min-width:0;overflow:hidden;color:#dce3ea!important;font-family:var(--ha-font-family-body)!important;font-size:var(--zone-font-text,14px)!important;font-weight:var(--ha-font-weight-medium)!important;line-height:1;text-overflow:ellipsis;white-space:nowrap}#page-home .machine-compact-head .machine-auto-off{flex:0 0 auto;min-height:20px;padding:3px 6px;font-size:9px}#page-home .machine-mini-metrics{min-width:0;display:grid}#page-home .machine-mini-metric{position:relative;min-width:0;display:grid;align-items:center;overflow:hidden;border:1px solid #303a45;background:#151b21;box-sizing:border-box}#page-home .machine-mini-icon{display:grid;place-items:center;background:#202832;color:#8d99a6}#page-home .home-temperature-hero-icon svg{width:16px;height:16px}#page-home .home-temperature-hero-icon svg,#page-home .machine-mini-icon svg{fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}#page-home .mini-pressure .machine-mini-icon{color:#76bfff}#page-home .mini-backflush .machine-mini-icon{color:#b29aeb}#page-home .machine-mini-copy{display:flex;flex-direction:column}#page-home .machine-mini-copy small,#page-home .machine-mini-copy strong{max-width:100%;overflow:hidden;font-weight:var(--ha-font-weight-medium);line-height:1;text-overflow:ellipsis;white-space:nowrap}#page-home .machine-mini-copy strong{display:flex;align-items:baseline;gap:3px}#page-home .home-temperature-hero-reading strong i,#page-home .machine-mini-copy strong em,#page-home .machine-mini-copy strong i{font-style:normal}#page-home .machine-mini-copy strong em{color:#7f8a97}#page-home .machine-mini-copy strong b{color:#697582;font-size:10px;font-weight:var(--ha-font-weight-normal)}#page-home .mini-water>#machineWaterIcon,#page-home .mini-water>#machineWaterText{position:absolute!important;width:1px!important;height:1px!important;overflow:hidden!important;clip:rect(0 0 0 0)!important;clip-path:inset(50%)!important;white-space:nowrap!important}#page-home .water-state-dot{display:block;border-radius:50%;background:#68737f;box-shadow:0 0 0 3px rgba(104,115,127,.12)}#page-home .mini-water.ok{border-color:#315b44;background:#15251d}#page-home .mini-water.ok .machine-mini-icon{color:#62d595}#page-home .mini-water.ok .water-state-dot{background:#62d595;box-shadow:0 0 0 3px rgba(98,213,149,.13)}#page-home .mini-water.warn{border-color:#6a3d42;background:#2b1d20}#page-home .mini-water.warn .machine-mini-icon{color:#ef7b83}#page-home .mini-water.warn .water-state-dot{background:#ef7b83;box-shadow:0 0 0 3px rgba(239,123,131,.13)}#page-home .home-temperature-hero{position:relative;min-width:0;padding:0!important;overflow:hidden;border:1px solid #34404c!important;border-radius:14px!important;background:radial-gradient(circle at 82% -20%,rgba(225,139,72,.16),transparent 47%),linear-gradient(155deg,#1b222a,#13181e 72%)!important;box-shadow:inset 0 1px 0 rgba(255,255,255,.04),0 6px 18px rgba(0,0,0,.18)!important;cursor:pointer}#page-home .home-temperature-hero:focus-visible,#page-home .home-temperature-hero:hover{border-color:#725039!important;outline:0;box-shadow:inset 0 1px 0 rgba(255,255,255,.05),0 0 0 3px rgba(225,139,72,.08),0 7px 20px rgba(0,0,0,.2)!important}#page-home .home-temperature-hero-state{height:100%;display:grid;box-sizing:border-box}#page-home .home-temperature-hero-head{min-width:0;display:flex;align-items:center;justify-content:space-between;gap:10px}#page-home .home-temperature-hero-title{min-width:0;display:flex;align-items:center;gap:7px;color:#b7c0ca;font-size:var(--zone-font-text,12px);font-weight:var(--ha-font-weight-medium);line-height:1;white-space:nowrap}#page-home .home-temperature-hero-icon{width:24px;min-width:24px;height:24px;display:grid;place-items:center;border-radius:7px;background:#29241f;color:#e7a06a}#page-home .home-temperature-hero-badges{min-width:0;display:flex;align-items:center;justify-content:flex-end;gap:5px}#page-home .home-temperature-pid,#page-home .home-temperature-state-label{min-height:21px;display:inline-flex;align-items:center;padding:3px 7px;border:1px solid #394550;border-radius:999px;background:#1b2229;color:#9da8b4;font-size:8px;font-weight:var(--ha-font-weight-medium);line-height:1;white-space:nowrap}#page-home .home-temperature-pid{color:#d4dbe3}#page-home .home-temperature-pid i{margin:0 2px 0 4px;color:#efae7b;font-style:normal;font-weight:var(--ha-font-weight-bold)}#page-home .home-temperature-open{width:22px;height:22px;display:grid;place-items:center;border-radius:7px;background:#242c34;color:#8f9aa7;font-size:10px}#page-home .home-temperature-hero-reading strong{min-width:0;display:flex;align-items:baseline;gap:5px;color:#f0c5a4;font-weight:var(--ha-font-weight-medium);line-height:.95;letter-spacing:-.035em;white-space:nowrap}#page-home .home-temperature-hero-reading strong em{color:#8e99a6;font-size:12px;font-style:normal;font-weight:var(--ha-font-weight-medium);letter-spacing:0}#page-home .home-temperature-target{flex:0 0 auto;font-size:9px}#page-home .home-temperature-mini-chart{position:relative;min-width:0;overflow:hidden;border-top:1px solid rgba(255,255,255,.035);border-bottom:1px solid rgba(255,255,255,.035)}#page-home #homePressureGauge,#page-home #homeTempMiniChart{width:100%;height:100%;display:block}#page-home .home-temperature-hero-footer{min-width:0;display:grid;align-items:center;font-size:8px;line-height:1}#page-home .home-temperature-heat-track{background:#28313a}#page-home .home-temperature-heat-track>#homeHeatFill{width:0;height:100%;border-radius:inherit;background:linear-gradient(90deg,#d97852,#e9b06e);transition:width .2s ease}#page-home .home-temperature-delta{min-width:0;overflow:hidden;text-align:right;text-overflow:ellipsis;white-space:nowrap}#page-home .home-temperature-hero-state.ready .home-temperature-state-label{border-color:#35664d;background:#182a21;color:#91eab5}#page-home .home-temperature-hero-state.heating .home-temperature-state-label{border-color:#70512f;background:#2b2118;color:#ffc27f}#page-home .home-temperature-hero-state.error .home-temperature-state-label{border-color:#743c43;background:#321f23;color:#ff9ba2}@media (max-width:900px) and (min-width:701px){#page-home .home-temperature-state-label,#page-home .machine-mini-copy small{display:none}#page-home .machine-mini-metric{grid-template-columns:22px minmax(0,1fr);gap:3px;padding:4px}#page-home .machine-mini-icon{width:22px;height:22px}#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,1fr))!important;padding:0 2px}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:100%!important;min-width:0!important;max-width:none!important}}@media (max-width:700px){#page-home .home-top-grid>.home-temperature-hero,#page-home .home-top-grid>.machine-panel{height:153px!important}}@media (max-width:430px){#page-home .home-temperature-open,#page-home .home-temperature-state-label,#page-home .machine-mini-copy small{display:none}#page-home .machine-mini-metric{grid-template-columns:20px minmax(0,1fr);gap:3px;padding:3px 4px}#page-home .machine-mini-icon{width:20px;height:20px}#page-home .machine-mini-icon svg{width:14px;height:14px}#page-home .machine-mini-copy strong{font-size:11px!important}#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,1fr))!important;gap:6px!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:100%!important;min-width:0!important;max-width:none!important}#page-home .home-temperature-hero-reading strong{font-size:27px!important}#page-home .home-temperature-delta{max-width:92px}}@media (min-width:701px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{gap:7px!important}}#page-home .machine-compact-status{position:relative;display:block!important;padding:6px!important}#page-home .machine-compact-head{display:none!important}#page-home .machine-mini-metrics{width:100%;height:100%;min-height:0;grid-template-columns:repeat(3,minmax(0,1fr));gap:6px}#page-home .machine-mini-metric{height:100%;min-height:0;grid-template-columns:minmax(0,1fr);grid-template-rows:31px minmax(0,1fr);place-items:center;align-content:center;gap:5px;padding:7px 5px;text-align:center;border-radius:10px}#page-home .machine-mini-icon{width:31px;height:31px;border-radius:9px}#page-home .machine-mini-icon svg{width:19px;height:19px}#page-home .machine-mini-copy{width:100%;min-width:0;align-items:center;justify-content:flex-start;gap:4px}#page-home .machine-mini-copy small{width:100%;color:#8d98a5;font-size:9px!important;text-align:center}#page-home .machine-mini-copy strong{width:100%;justify-content:center;color:#e0e7ee;font-size:16px!important;text-align:center}#page-home .machine-mini-copy strong em{font-size:9px}#page-home .water-state-dot{width:10px;height:10px}#page-home .machine-auto-off-overlay{position:absolute;top:5px;right:5px;z-index:5;min-height:20px;padding:3px 6px;border:1px solid #47515d;border-radius:999px;background:#222a32;box-shadow:0 3px 9px rgba(0,0,0,.25);font-size:8px}@media (max-width:900px) and (min-width:701px){#page-home .machine-mini-metric{grid-template-rows:28px minmax(0,1fr);gap:4px;padding:6px 3px}#page-home .machine-mini-icon{width:28px;height:28px}#page-home .machine-mini-copy small{display:block!important;font-size:8px!important}#page-home .machine-mini-copy strong{font-size:13px!important}}@media (max-width:700px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{gap:7px!important}}@media (max-width:430px){#page-home .machine-mini-metrics{gap:4px}#page-home .machine-mini-metric{grid-template-rows:25px minmax(0,1fr);gap:3px;padding:5px 2px}#page-home .machine-mini-icon{width:25px;height:25px}#page-home .machine-mini-icon svg{width:16px;height:16px}#page-home .machine-mini-copy small{display:block!important;font-size:7px!important}#page-home .machine-mini-copy strong{font-size:11px!important}}#page-home .home-top-grid>.home-temperature-hero,#page-home .home-top-grid>.machine-panel{min-height:165px!important;height:165px!important}#page-home .home-top-grid>.machine-panel>.machine-control-card{min-height:165px!important;height:165px!important;grid-template-rows:78px 80px!important;gap:7px!important}#page-home .machine-restored-status{position:relative;display:flex!important;align-items:center!important;justify-content:space-between!important;gap:12px!important;height:78px!important;min-height:78px!important;padding:11px 13px!important;overflow:hidden!important;box-sizing:border-box}#page-home .machine-restored-status .machine-status-copy{min-width:0;display:flex;flex-direction:column;align-items:flex-start;justify-content:center;gap:7px}#page-home .machine-restored-status .machine-status-copy>#shotStatus{display:block!important;max-width:100%;margin:0;padding:0;overflow:hidden;border:0!important;border-radius:0!important;background:0 0!important;box-shadow:none!important;color:#d8dee7!important;font-family:var(--ha-font-family-body)!important;font-size:var(--zone-font-title,16px)!important;font-weight:var(--ha-font-weight-medium)!important;line-height:var(--ha-line-height-condensed)!important;letter-spacing:0!important;text-overflow:ellipsis;white-space:nowrap}#page-home .machine-restored-status .machine-auto-off{min-height:0!important;margin:0!important;padding:0!important;border:0!important;background:0 0!important;color:#8f9aa7!important;box-shadow:none!important}#page-home .machine-restored-status .machine-water-indicator{position:relative;flex:0 0 auto;width:38px!important;min-width:38px!important;height:38px!important;min-height:38px!important;display:grid!important;place-items:center!important;padding:0!important;border:1px solid #35404c!important;border-radius:11px!important;background:#1c232b!important;box-shadow:inset 0 1px 0 rgba(255,255,255,.035)!important}#page-home .machine-restored-status .machine-water-drop{width:24px;height:24px;display:grid;place-items:center;color:#74808d}#page-home .machine-restored-status .machine-water-drop svg{width:22px;height:22px;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}#page-home .machine-restored-status .machine-water-label,#page-home .machine-restored-status .machine-water-result{position:absolute!important;width:1px!important;height:1px!important;overflow:hidden!important;clip:rect(0 0 0 0)!important;clip-path:inset(50%)!important;white-space:nowrap!important}#page-home .machine-restored-status .machine-water-indicator.ok{border-color:#35694c!important;background:#193126!important}#page-home .machine-restored-status .machine-water-indicator.ok .machine-water-drop{color:#63d493!important}#page-home .machine-restored-status .machine-water-indicator.warn{border-color:#774047!important;background:#352126!important}#page-home .machine-restored-status .machine-water-indicator.warn .machine-water-drop{color:#f27a82!important}#page-home .machine-restored-status .machine-water-indicator.unknown{border-color:#35404c!important;background:#1c232b!important}#page-home .machine-restored-status .machine-water-indicator.unknown .machine-water-drop{color:#74808d!important}#page-home .machine-actions-dock,#page-home .machine-actions-dock .machine-quick-actions{height:80px!important;min-height:80px!important}#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,123px)!important;grid-auto-rows:80px!important;justify-content:center!important;gap:10px!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:123px!important;min-width:123px!important;max-width:123px!important;height:80px!important;min-height:80px!important;max-height:80px!important}#page-home .machine-actions-dock .machine-quick-actions .quick-icon{width:58px!important;height:58px!important}@media (max-width:900px) and (min-width:701px){#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,1fr))!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:100%!important;min-width:0!important;max-width:none!important}}@media (max-width:700px){#page-home .home-top-grid>.home-temperature-hero,#page-home .home-top-grid>.machine-panel{min-height:165px!important;height:165px!important}}@media (max-width:430px){#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,1fr))!important;gap:6px!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:100%!important;min-width:0!important;max-width:none!important}#page-home .machine-restored-status{padding:9px 10px!important}#page-home .machine-restored-status .machine-water-indicator{width:34px!important;min-width:34px!important;height:34px!important;min-height:34px!important}}#page-home .home-top-grid>.home-temperature-hero,#page-home .home-top-grid>.machine-panel,#page-home .home-top-grid>.machine-panel>.machine-control-card{height:var(--home-temperature-card-height)!important;min-height:var(--home-temperature-card-height)!important}#page-home .home-top-grid>.machine-panel>.machine-control-card{grid-template-rows:78px minmax(80px,1fr)!important}#page-home .machine-actions-dock{align-items:center;justify-content:center}#page-home .home-temperature-hero-state{min-height:0}@media (max-width:1180px){.machine-icon-size-tools{grid-template-columns:repeat(2,minmax(210px,1fr))}.machine-icon-size-tools>button{grid-column:1/-1;width:100%}}@media (max-width:620px){.machine-icon-size-tools{grid-template-columns:minmax(0,1fr)}.machine-icon-size-tools>button{grid-column:auto}}@media (min-width:701px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{grid-template-columns:minmax(0,var(--home-left-column-share)) minmax(0,var(--home-right-column-share))!important}}.machine-icon-size-tools{grid-template-columns:repeat(5,minmax(170px,1fr)) auto}@media (max-width:1320px){.machine-icon-size-tools{grid-template-columns:repeat(3,minmax(190px,1fr))}.machine-icon-size-tools>button{grid-column:1/-1;width:100%}}@media (max-width:920px){.machine-icon-size-tools{grid-template-columns:repeat(2,minmax(210px,1fr))}}@media (max-width:620px){.machine-icon-size-tools{grid-template-columns:minmax(0,1fr)}.machine-icon-size-tools>button{grid-column:auto}}#page-settings [data-settings-panel=pressure]{width:100%;margin:0 auto}.pressure-parameter-grid,.pressure-settings-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr))}.pressure-settings-grid{gap:10px;margin-top:12px}.pressure-parameter-grid{gap:8px}#page-settings [data-settings-panel=pressure] .temperature-setting-control{grid-template-columns:minmax(82px,138px) auto;justify-content:end}#page-settings [data-settings-panel=pressure] .temperature-status-row>strong,#page-settings [data-settings-panel=pressure] .temperature-status-row>strong i{font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);font-style:normal;font-variant-numeric:tabular-nums}.pressure-settings-save{margin-top:10px}@media (max-width:760px){#page-settings [data-settings-panel=pressure]{max-width:none}.pressure-parameter-grid,.pressure-settings-grid{grid-template-columns:minmax(0,1fr)}.pressure-settings-section-wide{grid-column:1}}@media (max-width:520px){#page-settings [data-settings-panel=pressure] .temperature-setting-control{grid-template-columns:minmax(0,1fr) auto;justify-content:stretch}}@media (max-width:760px){}@media (max-width:520px){}.machine-settings-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:10px;margin-top:12px}.machine-settings-section-wide{grid-column:1/-1}.machine-setting-control,.settings-help-label-wrap>b,.settings-help-label-wrap>label{min-width:0}#page-settings .machine-setting-control select{width:100%;min-width:0;min-height:36px;padding:7px 9px;border-radius:8px;font-size:var(--zone-font-text)!important}.machine-autooff-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:0 18px}.machine-inline-actions{display:flex;justify-content:flex-end;padding-top:9px;border-top:1px solid #29313a}#page-settings .machine-inline-actions button,#page-settings .machine-primary-actions button,#page-settings .machine-settings-save button{min-height:36px;padding:7px 12px;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium)}.machine-settings-save{margin-top:10px}#page-settings .temperature-status-row>strong i{font-style:normal;font-variant-numeric:tabular-nums}.settings-help-all-button{flex:0 0 auto;min-height:30px;padding:5px 9px;border:1px solid #343e49;border-radius:9px;background:#171d23;color:#aeb7c1;font-size:var(--zone-font-small)!important;font-weight:var(--ha-font-weight-medium);line-height:1}.settings-help-all-button.active,.settings-help-all-button:hover{border-color:#596676;background:#1c232a;color:#e2e7ec}.settings-help-label-wrap,.xdb-human-chips>span{min-width:0;display:inline-flex;align-items:center;gap:6px}.settings-info-button{flex:0 0 auto;width:19px;height:19px;display:inline-grid;place-items:center;padding:0;border:1px solid #3a4550;border-radius:50%;background:0 0;color:#82909e;font-size:11px!important;font-weight:700;line-height:1}.settings-info-button.active,.settings-info-button:hover{border-color:#687788;background:#202831;color:#e4e9ee}.settings-help-panel .settings-help-text{display:none!important}.settings-help-panel .settings-help-owner[data-help-open=true]>.settings-help-text,.settings-help-panel.help-all-open .settings-help-text{display:block!important}.settings-help-panel .settings-help-owner[data-help-open=true]>.settings-help-text{animation:settingsHelpReveal .14s ease-out}.temperature-settings-section-head>.settings-help-label-wrap{justify-self:start}.pid-parameter-main>.settings-help-label-wrap{flex:0 1 auto}@media (max-width:760px){#page-settings [data-settings-panel=machine]{max-width:none}.machine-autooff-grid,.machine-settings-grid{grid-template-columns:minmax(0,1fr)}.machine-settings-section-wide{grid-column:1}}@media (max-width:520px){.settings-panel-head{align-items:center}.settings-help-all-button{padding:5px 8px}.machine-inline-actions button{width:100%}}#page-settings [data-settings-panel=machine]{width:100%;margin:0 auto}.machine-primary-grid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:9px;margin-top:12px}.machine-primary-row{margin:0;padding:11px 12px;border:1px solid #303944!important;border-radius:12px;background:#12171c;grid-template-columns:minmax(0,1fr) 150px}.machine-primary-row .temperature-setting-copy{gap:3px}#page-settings .machine-primary-row .temperature-setting-copy>label{font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium)}.machine-live-meta{color:#84909d;font-size:var(--zone-font-small)!important;line-height:1.25}.machine-live-meta b{color:#b8c2cc;font-size:inherit!important;font-weight:var(--ha-font-weight-medium);font-variant-numeric:tabular-nums}.machine-primary-actions{display:flex;justify-content:flex-end;gap:8px;margin-top:10px}#page-settings [data-settings-panel=machine] .settings-panel-head{margin-bottom:0}@media (max-width:760px){#page-settings [data-settings-panel=machine]{max-width:none}.machine-primary-grid{grid-template-columns:minmax(0,1fr)}}@media (max-width:520px){.machine-primary-row{grid-template-columns:minmax(0,1fr) 132px;padding:10px}.machine-primary-actions{display:grid;grid-template-columns:minmax(0,1fr)}#page-settings .machine-primary-actions button{width:100%}}#page-settings [data-settings-panel=machine],#page-settings [data-settings-panel=pressure],#page-settings [data-settings-panel=temperature]{max-width:720px}.machine-primary-grid,.pid-parameter-grid,.pressure-parameter-grid,.pressure-settings-grid,.temperature-settings-grid{grid-template-columns:minmax(0,1fr)!important}.pressure-settings-section-wide,.temperature-settings-section-wide{grid-column:1!important}.machine-primary-row{min-height:54px}.settings-info-button{display:none!important}.settings-help-label-wrap{display:contents}@media (max-width:760px){#page-settings [data-settings-panel=machine],#page-settings [data-settings-panel=pressure],#page-settings [data-settings-panel=temperature]{max-width:none}}@media (min-width:980px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{grid-template-columns:minmax(350px,var(--home-left-column-share)) minmax(155px,var(--home-pressure-column-share)) minmax(290px,var(--home-right-column-share))!important;gap:7px!important}#page-home .machine-actions-dock .machine-quick-actions{grid-template-columns:repeat(3,minmax(0,123px))!important;justify-content:space-between!important}#page-home .machine-actions-dock .machine-quick-actions .quick-action,#page-home .machine-actions-dock .machine-quick-actions .quick-action.active,#page-home .machine-actions-dock .machine-quick-actions .quick-action.danger,#page-home .machine-actions-dock .machine-quick-actions .quick-action:active,#page-home .machine-actions-dock .machine-quick-actions .quick-action:disabled,#page-home .machine-actions-dock .machine-quick-actions .quick-action:hover{width:100%!important;min-width:0!important;max-width:123px!important}}#page-home .home-pressure-hero{position:relative;min-width:0;height:var(--home-temperature-card-height)!important;min-height:var(--home-temperature-card-height)!important;gap:3px;overflow:hidden;border:1px solid #344556;border-radius:14px;background:radial-gradient(circle at 50% 105%,rgba(69,143,201,.17),transparent 58%),linear-gradient(155deg,#19222b,#12181e 74%);box-shadow:inset 0 1px 0 rgba(255,255,255,.04),0 6px 18px rgba(0,0,0,.17);box-sizing:border-box}#page-home .home-pressure-hero::before{position:absolute;top:0;left:19px;right:19px;height:2px;border-radius:0 0 999px 999px;background:#5da9ef;opacity:.82}#page-home .home-pressure-hero.live{border-color:#3d6482}#page-home .home-pressure-head{min-width:0;display:flex;align-items:center;justify-content:space-between;gap:6px}#page-home .home-pressure-reading strong,#page-home .home-pressure-title{font-family:var(--ha-font-family-body);font-weight:var(--ha-font-weight-medium)}#page-home .home-pressure-title{min-width:0;display:flex;align-items:center;gap:6px;color:#b7c3cf;font-size:10px;line-height:1;white-space:nowrap}#page-home .home-pressure-icon{width:24px;min-width:24px;height:24px;display:grid;place-items:center;border-radius:7px;background:#1f2d39;color:#70b9f5}#page-home .home-pressure-icon svg,.diagnostics-system-section-icon svg,.xdb-human-icon svg{width:16px;height:16px;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round}#page-home .home-pressure-range{flex:0 0 auto;color:#748392;font-size:7px;line-height:1;white-space:nowrap}#page-home .home-pressure-reading{z-index:2;display:flex;align-items:baseline;transform:translateX(-50%);white-space:nowrap;pointer-events:none}#page-home .home-pressure-reading strong{color:#b7dcfa}#page-home .home-pressure-reading em{color:#8593a1;font-style:normal;font-weight:var(--ha-font-weight-medium);font-size:10px!important}#page-home .home-pressure-footer{min-width:0;display:grid;grid-template-columns:auto minmax(0,1fr) auto;align-items:center;gap:4px;color:#6f7d8b;font-size:7px;line-height:1}#page-home .home-pressure-target{overflow:hidden;color:#8593a1;text-align:center;text-overflow:ellipsis;white-space:nowrap}#page-home .home-pressure-target b,.diagnostics-section-label,.xdb-human-copy>b{color:#72dda0;font-size:9px;font-weight:var(--ha-font-weight-medium)}@media (min-width:701px) and (max-width:979px){#page-home .home-top-grid,#page-home .home-top-grid.metrics-first{grid-template-columns:minmax(0,.72fr) minmax(0,1.28fr)!important;gap:7px!important}#page-home .home-top-grid>.machine-panel{grid-column:1/-1}#page-home .home-pressure-hero{grid-column:1}#page-home .home-temperature-hero{grid-column:2}}@media (max-width:700px){#page-home .home-pressure-hero{height:var(--home-temperature-card-height)!important;min-height:var(--home-temperature-card-height)!important}}@media (max-width:430px){#page-home .home-pressure-reading strong{font-size:26px}#page-home .home-pressure-title{font-size:9px}}#page-home .home-pressure-hero{display:grid!important;grid-template-columns:minmax(0,1fr)!important;grid-template-rows:minmax(0,1fr)!important;place-items:center!important;padding:12px!important}#page-home .home-pressure-footer,#page-home .home-pressure-head,#page-home .home-temperature-heat-track>#homeHeatFill{display:none!important}#page-home .home-pressure-gauge{position:relative;min-width:0;align-self:center;justify-self:stretch;overflow:hidden}#page-home .home-pressure-reading{position:absolute!important}#page-home .home-pressure-reading strong{font-size:clamp(25px,2.15vw,34px)}@media (max-width:430px){#page-home .home-pressure-gauge{min-height:90px}#page-home .home-pressure-reading strong{font-size:28px}}#page-home .home-pressure-hero{background:radial-gradient(circle at 50% 48%,rgba(68,139,191,.13),transparent 54%),linear-gradient(155deg,#19222b,#12181e 74%)!important}#page-home .home-pressure-gauge{width:100%!important;height:calc(var(--home-temperature-card-height) - 18px)!important;min-height:112px;max-height:242px}#page-home .home-pressure-reading{top:74%!important;left:50%!important;bottom:auto!important;gap:4px!important;transform:translate(-50%,-50%)!important}#page-home .home-pressure-reading strong{font-size:31px!important;line-height:.9!important;letter-spacing:-.035em}@media (max-width:430px){#page-home .home-pressure-gauge{min-height:105px}#page-home .home-pressure-reading strong{font-size:31px!important}}#page-home .home-pressure-hero::before,#page-home .pressure-variant-a::before{content:none!important;display:none!important}#page-home .pressure-variant-a{padding:6px!important;background:radial-gradient(circle at 50% 46%,rgba(70,146,202,.16),transparent 58%),linear-gradient(155deg,#19222b,#12181e 74%)!important}#page-home .pressure-variant-a .home-pressure-gauge{width:100%!important;height:100%!important;min-height:122px;max-height:none}#page-home .pressure-variant-a .pressure-reading-stacked{top:76%!important;left:50%!important;bottom:auto!important;display:flex!important;flex-direction:column;align-items:center;gap:2px!important;transform:translate(-50%,-50%)!important}#page-home .pressure-variant-a .pressure-reading-stacked strong{font-size:36px!important;line-height:.82!important}#page-home .pressure-variant-a .pressure-reading-stacked em{font-size:9px!important;letter-spacing:.08em;text-transform:uppercase}.diagnostics-section-label{margin-top:13px;color:#7f8b97;font-size:10px;letter-spacing:.055em;text-transform:uppercase}.xdb-human-card{display:grid;grid-template-columns:42px minmax(0,1fr);align-items:start;gap:11px;margin-top:12px;padding:12px 13px;border:1px solid #35424e;border-radius:12px;background:#151b21;transition:border-color .18s ease,background .18s ease}.xdb-human-icon{width:42px;height:42px;display:grid;place-items:center;border-radius:11px;background:#222b34;color:#91a0ae}.diagnostics-system-section-icon svg,.xdb-human-icon svg{width:23px;height:23px}.xdb-human-copy{min-width:0}.xdb-human-copy>b{display:block;color:#dce4ec;font-size:15px;line-height:1.2}.xdb-human-copy>p{margin:5px 0 0;color:#8e9aa6;font-size:11px;line-height:1.45}.xdb-human-chips{display:flex;flex-wrap:wrap;gap:6px;margin-top:9px}.xdb-human-chips>span{gap:4px;padding:5px 7px;border:1px solid #303b46;border-radius:8px;background:#11171c;color:#758390;font-size:9px;line-height:1}.xdb-human-chips b,.xdb-shot-head>div>span{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}.xdb-human-chips b{max-width:240px;color:#abb7c2;font-weight:var(--ha-font-weight-medium)}.xdb-human-card.ok{border-color:rgba(76,184,127,.38);background:linear-gradient(135deg,rgba(54,139,96,.12),rgba(21,27,33,.96) 56%)}.xdb-human-card.ok .xdb-human-icon{background:rgba(54,139,96,.15);color:#68d49b}.xdb-human-card.warning{border-color:rgba(220,174,73,.43);background:linear-gradient(135deg,rgba(178,127,45,.13),rgba(21,27,33,.96) 58%)}.xdb-human-card.warning .xdb-human-icon{background:rgba(178,127,45,.16);color:#e6bd68}.xdb-human-card.critical{border-color:rgba(224,99,86,.5);background:linear-gradient(135deg,rgba(178,70,61,.16),rgba(21,27,33,.97) 58%)}.xdb-human-card.critical .xdb-human-icon{background:rgba(178,70,61,.18);color:#f08073}.xdb-shot-card{margin-top:13px;padding:12px 13px;border:1px solid #303b46;border-radius:12px;background:#12181e}.xdb-shot-head{display:flex;align-items:center;justify-content:space-between;gap:10px}.xdb-shot-facts>div,.xdb-shot-head>div{min-width:0;display:flex;flex-direction:column;gap:3px}.xdb-shot-facts b,.xdb-shot-head>div>b{color:#cfd8e1;font-size:12px;font-weight:var(--ha-font-weight-medium)}.xdb-shot-head>div>span{color:#75828f;font-size:9px}.xdb-shot-head button{flex:0 0 auto}.xdb-shot-card>p{margin:10px 0 0;color:#929eaa;font-size:11px;line-height:1.5}.xdb-shot-card.problem{border-color:rgba(224,99,86,.38)}.xdb-shot-card.clear,.xdb-system-summary.ok{border-color:rgba(76,184,127,.32)}.xdb-shot-facts{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:7px;margin-top:11px}.xdb-shot-facts>div{gap:4px;padding:8px 9px;border:1px solid #2d3741;border-radius:9px;background:#0f151a}.xdb-shot-facts span{color:#71808d;font-size:8px;line-height:1.2}.xdb-shot-facts b{overflow:hidden;color:#bcc7d1;font-size:11px;line-height:1.25;text-overflow:ellipsis}.xdb-system-summary{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-top:12px;padding:11px 12px;border:1px solid #35404b;border-radius:11px;background:#141a20}.diagnostics-system-section-head>div,.xdb-system-summary>div{min-width:0}.xdb-system-summary b{color:#d2dbe3;font-size:12px;font-weight:var(--ha-font-weight-medium)}.xdb-system-summary p{margin:4px 0 0;color:#82909c;font-size:10px;line-height:1.4}.xdb-system-summary>span{flex:0 0 auto;max-width:180px;padding:6px 8px;overflow:hidden;border:1px solid #303b46;border-radius:8px;background:#10161b;color:#9eabb7;font-size:9px;text-align:right;text-overflow:ellipsis;white-space:nowrap}.xdb-system-summary.warning{border-color:rgba(220,174,73,.38)}.xdb-system-summary.critical{border-color:rgba(224,99,86,.45)}@media (max-width:620px){.xdb-shot-facts{grid-template-columns:minmax(0,1fr)}.xdb-shot-head,.xdb-system-summary{align-items:stretch;flex-direction:column}.xdb-shot-head button,.xdb-system-summary>span{width:100%;max-width:none}.xdb-system-summary>span{text-align:left}}@media (max-width:430px){.xdb-human-card{grid-template-columns:36px minmax(0,1fr);padding:11px}.xdb-human-icon{width:36px;height:36px}}.diagnostics-system-section{margin-top:12px;padding:12px;border:1px solid #303b46;border-radius:13px;background:#12181e}.diagnostics-system-section+.diagnostics-system-section{margin-top:10px}.diagnostics-system-section-head{display:grid;grid-template-columns:38px minmax(0,1fr) auto;align-items:center;gap:10px}.diagnostics-system-section-head>div>b{display:block;color:#d7e0e8;font-size:13px;line-height:1.2}.diagnostics-system-section-head>div>p{margin:3px 0 0;color:#7f8c98;font-size:9px;line-height:1.35}.diagnostics-system-section-icon{width:38px;height:38px;display:grid;place-items:center;border-radius:10px}.diagnostics-system-section-icon svg{width:21px;height:21px;stroke-width:1.7}.diagnostics-system-section-icon.sensor{background:rgba(70,139,192,.14);color:#70b9ee}.diagnostics-system-section-icon.esp{background:rgba(132,107,184,.14);color:#b19ae2}.diagnostics-section-health{min-width:0;padding:6px 8px;border:1px solid #303b46;border-radius:8px;background:#0f151a;color:#75828e;font-size:9px;white-space:nowrap}#page-home .home-temperature-target b,.diagnostics-section-health b,.diagnostics-system-section-head>div>b{font-weight:var(--ha-font-weight-medium)}.diagnostics-system-section .xdb-system-summary{margin-top:11px}.diagnostics-system-section .diagnostics-section-label{margin-top:12px}.diagnostics-esp-grid,.diagnostics-sensor-grid{margin-top:8px}.diagnostics-esp-grid>div,.diagnostics-sensor-grid>div{min-height:64px}.diagnostics-esp-note{margin:9px 1px 0;color:#74818d;font-size:9px;line-height:1.4}.diagnostics-system-actions{margin-top:11px}#shotChartCard .home-archive-summary{grid-template-columns:repeat(4,minmax(0,1fr));gap:6px;margin:0 0 7px}#shotChartCard .home-archive-summary .home-archive-metric{min-width:0;min-height:49px;padding:7px 8px;border:1px solid #3c4552;border-radius:10px;background:linear-gradient(145deg,#171c23,#11151a);color:var(--text);text-align:left;box-shadow:none}#shotChartCard .home-archive-summary .home-archive-metric b,#shotChartCard .home-archive-summary .home-archive-metric small{display:block;overflow:hidden;color:#9ca6b2;font-size:var(--zone-font-chart)!important;font-weight:var(--ha-font-weight-medium);line-height:1.1;text-overflow:ellipsis;white-space:nowrap}#shotChartCard .home-archive-summary .home-archive-metric b{margin-top:3px;color:#e7ebf0;font-size:var(--zone-font-value)!important;line-height:1}@media (max-width:620px){.diagnostics-system-section-head{grid-template-columns:38px minmax(0,1fr)}.diagnostics-section-health{grid-column:1/-1}#shotChartCard .home-archive-summary{grid-template-columns:repeat(2,minmax(0,1fr))}}@media (max-width:390px){#shotChartCard .home-archive-summary{grid-template-columns:minmax(0,1fr)}}@media (min-width:981px){.shot-profile-board,.shot-profile-board.line-settings-open{align-items:start!important}.shot-profile-board.line-settings-open>#shotChartCard,.shot-profile-board>#shotChartCard{align-self:start!important;height:auto!important;min-height:520px}.shot-profile-board.line-settings-open>#profileLibraryCard,.shot-profile-board>#profileLibraryCard{align-self:start!important;height:520px!important;min-height:520px!important;max-height:520px!important}#profileLibraryCard .home-shot-rail-pane{min-height:0}#profileLibraryCard .home-archive-list,#profileLibraryCard .user-profile-list{min-height:0;overflow:auto}}#page-home .home-temperature-hero-state{grid-template-rows:35px minmax(58px,1fr) 23px!important;gap:6px!important;padding:10px 12px!important}#page-home .home-temperature-hero-reading{min-width:0;display:flex;align-items:baseline;justify-content:space-between;gap:14px;padding:0 2px}#page-home .home-temperature-hero-reading strong{font-size:32px!important}#page-home .home-temperature-target{color:#98a3af;font-size:var(--zone-font-text,14px)!important;font-weight:var(--ha-font-weight-medium);line-height:1;white-space:nowrap}#page-home .home-temperature-target b{color:#a9e2be;font-size:32px!important;letter-spacing:-.02em}#page-home .home-temperature-mini-chart{min-height:58px}#page-home .home-temperature-hero-footer{grid-template-columns:auto minmax(72px,1fr) minmax(0,auto);gap:9px;color:#9aa5b1;font-size:var(--zone-font-small,10px)!important;font-weight:var(--ha-font-weight-medium)}#page-home .home-temperature-hero-footer>span:first-child{color:#b8c1cb}#page-home .home-temperature-heat-track{--heat-pct:0%;position:relative;height:6px;overflow:hidden;border-radius:999px;background:linear-gradient(90deg,#c9aa58 0,#df9a4e 52%,#e87c4f 78%,#ee6952 100%);box-shadow:inset 0 0 0 1px rgba(255,255,255,.035)}#page-home .home-temperature-heat-track::after{content:"";position:absolute;top:0;right:0;bottom:0;width:calc(100% - var(--heat-pct));background:#29323b;transition:width .2s ease}#page-home .home-temperature-delta{max-width:160px;color:#aab4bf;font-size:inherit;font-weight:var(--ha-font-weight-medium)}@media (max-width:430px){#page-home .home-temperature-hero-state{grid-template-rows:32px minmax(54px,1fr) 21px!important;padding:9px!important}#page-home .home-temperature-hero-reading strong,#page-home .home-temperature-target b{font-size:28px!important}#page-home .home-temperature-target{font-size:12px!important}#page-home .home-temperature-hero-footer{grid-template-columns:auto minmax(48px,1fr) minmax(0,auto);gap:6px;font-size:9px!important}#page-home .home-temperature-delta{max-width:108px}}#page-home .machine-mascot{--mascot-accent:#73808d;position:relative;flex:0 0 auto;width:60px;height:42px;display:grid;grid-template-columns:1fr 1fr;align-items:center;justify-items:center;column-gap:8px;padding:7px 9px 11px;border:1px solid #35404c;border-radius:13px;background:radial-gradient(circle at 50% 15%,rgba(255,255,255,.035),transparent 48%),#171e26;box-shadow:inset 0 1px 0 rgba(255,255,255,.04),0 0 0 1px rgba(0,0,0,.08);box-sizing:border-box;color:var(--mascot-accent);overflow:visible;transition:border-color .22s ease,background .22s ease,box-shadow .22s ease,color .22s ease}#page-home .machine-mascot::before{content:"";position:absolute;inset:5px;border-radius:9px;background:radial-gradient(circle at 50% 35%,color-mix(in srgb,var(--mascot-accent) 10%,transparent),transparent 68%);opacity:.75;pointer-events:none;transition:opacity .22s ease}#page-home .machine-mascot-eye{position:relative;z-index:1;width:15px;height:12px;display:grid;place-items:center;border:1.5px solid currentColor;border-radius:50%;background:rgba(9,12,16,.62);box-shadow:0 0 8px color-mix(in srgb,currentColor 16%,transparent);transform-origin:center;animation:machineMascotBlink 6.8s ease-in-out infinite;transition:height .2s ease,border-radius .2s ease,transform .2s ease,opacity .2s ease,background .2s ease}#page-home .machine-mascot-eye:nth-of-type(2){animation-delay:.13s}#page-home .machine-mascot-pupil{width:4px;height:4px;border-radius:50%;background:currentColor;box-shadow:0 0 6px color-mix(in srgb,currentColor 55%,transparent);animation:machineMascotLook 5.2s ease-in-out infinite;transition:transform .2s ease,opacity .2s ease}#page-home .machine-mascot-mouth{position:absolute;z-index:1;left:50%;bottom:6px;width:13px;height:5px;border-bottom:1.5px solid currentColor;border-radius:0 0 10px 10px;transform:translateX(-50%);opacity:.75;transition:width .2s ease,height .2s ease,border .2s ease,border-radius .2s ease,transform .2s ease,opacity .2s ease}#page-home .machine-mascot-steam{position:absolute;top:-7px;right:8px;width:20px;height:12px;opacity:0;pointer-events:none;transition:opacity .2s ease}#page-home .machine-mascot-steam i{position:absolute;bottom:0;width:2px;height:8px;border-radius:999px;background:currentColor;opacity:0;animation:machineMascotSteam 1.45s ease-out infinite}#page-home .machine-mascot-steam i:nth-child(1){left:2px;animation-delay:0s}#page-home .machine-mascot-steam i:nth-child(2){left:9px;animation-delay:.34s}#page-home .machine-mascot-steam i:nth-child(3){left:16px;animation-delay:.68s}#page-home .machine-mascot.state-off{--mascot-accent:#66717d;border-color:#303842;background:#161b21;opacity:.72}#page-home .machine-mascot.state-off .machine-mascot-eye{height:4px;border-radius:999px;border-width:1.5px 0 0;background:0 0;box-shadow:none;animation:none;transform:translateY(2px)}#page-home .machine-mascot.state-off .machine-mascot-pupil{opacity:0;animation:none}#page-home .machine-mascot.state-off .machine-mascot-mouth{width:10px;height:1px;border:0;border-top:1.5px solid currentColor;border-radius:0;opacity:.5}#page-home .machine-mascot.state-heating{--mascot-accent:#e59a55;border-color:#5b4432;background:#211b18;box-shadow:inset 0 1px 0 rgba(255,255,255,.04),0 0 15px rgba(225,139,72,.07);animation:machineMascotWarm 2.4s ease-in-out infinite}#page-home .machine-mascot.state-heating .machine-mascot-pupil{animation:machineMascotHeatLook 2.8s ease-in-out infinite}#page-home .machine-mascot.state-ready{--mascot-accent:#61d493;border-color:#37684d;background:#17251e}#page-home .machine-mascot.state-ready .machine-mascot-eye{height:10px}#page-home .machine-mascot.state-ready .machine-mascot-mouth{width:15px;height:7px;border-bottom-width:2px;opacity:1;animation:machineMascotSmile 3.2s ease-in-out infinite}#page-home .machine-mascot.state-shot{--mascot-accent:#ffd166;border-color:#6a592e;background:#241f16;animation:machineMascotFocus 1.25s ease-in-out infinite}#page-home .machine-mascot.state-shot .machine-mascot-eye{height:8px;border-radius:45%}#page-home .machine-mascot.state-shot .machine-mascot-pupil{animation:none;transform:translateY(1px) scale(.9)}#page-home .machine-mascot.state-shot .machine-mascot-mouth{width:8px;height:2px;border-radius:999px;opacity:.82}#page-home .machine-mascot.state-water{--mascot-accent:#62b9ff;border-color:#315b79;background:#17232c}#page-home .machine-mascot.state-water .machine-mascot-pupil{animation:machineMascotWaterLook 1.8s ease-in-out infinite}#page-home .machine-mascot.state-water .machine-mascot-mouth{width:6px;height:6px;border:1.5px solid currentColor;border-radius:50%;opacity:.85}#page-home .machine-mascot.state-steam{--mascot-accent:#b695ff;border-color:#574775;background:#201b2b}#page-home .machine-mascot.state-steam .machine-mascot-eye{width:16px;height:13px}#page-home .machine-mascot.state-steam .machine-mascot-steam{opacity:.9}#page-home .machine-mascot.state-steam .machine-mascot-steam i{opacity:.62}#page-home .machine-mascot.state-steam .machine-mascot-mouth{width:10px;height:6px;border-bottom-width:2px;opacity:1}#page-home .machine-mascot.state-warning{--mascot-accent:#f0a55f;border-color:#785139;background:#2a2119}#page-home .machine-mascot.state-warning .machine-mascot-eye:first-of-type{transform:rotate(-8deg)}#page-home .machine-mascot.state-warning .machine-mascot-eye:nth-of-type(2){transform:rotate(8deg)}#page-home .machine-mascot.state-warning .machine-mascot-mouth{width:14px;height:6px;border:0;border-top:1.8px solid currentColor;border-radius:10px 10px 0 0;transform:translateX(-50%) translateY(2px);opacity:1}#page-home .machine-mascot.state-error{--mascot-accent:#ff7777;border-color:#7c4247;background:#2b1b1f;box-shadow:inset 0 1px 0 rgba(255,255,255,.035),0 0 14px rgba(255,104,104,.08)}#page-home .machine-mascot.state-error .machine-mascot-eye{width:17px;height:14px;animation-duration:3.6s}#page-home .machine-mascot.state-error .machine-mascot-pupil{animation:none;transform:scale(.72)}#page-home .machine-mascot.state-error .machine-mascot-mouth{width:14px;height:6px;border:0;border-top:1.8px solid currentColor;border-radius:10px 10px 0 0;transform:translateX(-50%) translateY(2px);opacity:1}@media (max-width:760px){#page-home .machine-mascot{width:52px;height:38px;padding:6px 7px 10px;column-gap:6px}#page-home .machine-mascot-eye{width:13px;height:10px}}@media (prefers-reduced-motion:reduce){#page-home .machine-mascot,#page-home .machine-mascot *,#page-home .machine-mascot::before{animation:none!important;transition:none!important}}#page-home .pressure-variant-a .home-pressure-backflush{position:absolute;left:9px;right:9px;bottom:6px;z-index:4;min-width:0;display:flex;align-items:center;justify-content:center;gap:5px;padding:4px 7px 2px;border-top:1px solid rgba(118,145,167,.18);color:#788896;font-size:9px;font-weight:600;line-height:1.05;white-space:nowrap;pointer-events:none}#page-home .pressure-variant-a .home-pressure-backflush-icon{color:#7299b7;font-size:11px;line-height:1}#page-home .pressure-variant-a .home-pressure-backflush.due{color:#efb37c;border-top-color:rgba(225,139,72,.32)}#page-home .pressure-variant-a .home-pressure-backflush.due .home-pressure-backflush-icon{color:#e18b48}#shotChartCard .target-weight-footer.shot-result b{color:#d8eadf}#page-settings .pressure-settings-grid,#page-settings .temperature-settings-grid{gap:14px}#page-settings .temperature-settings-section{padding:0;border:0;border-radius:0;background:0 0}#page-settings .temperature-settings-section-head{margin:0 3px 7px;padding:0 1px;gap:3px}#page-maintenance .service-setting-copy label,#page-maintenance .toggle-copy span,#page-settings .temperature-settings-section-head>.settings-help-label-wrap>b,#page-settings .temperature-settings-section-head>b{color:#dce2e9;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);line-height:1.25}#page-settings .temperature-settings-section>.temperature-autotune-row,#page-settings .temperature-settings-section>.temperature-setting-row{margin:0;padding:10px 12px;border:1px solid #303944!important;border-radius:11px;background:#12171c}#page-settings .temperature-settings-section>.pid-parameter-grid+.temperature-autotune-row,#page-settings .temperature-settings-section>.pressure-parameter-grid+.temperature-setting-row,#page-settings .temperature-settings-section>.temperature-autotune-row+.temperature-setting-row,#page-settings .temperature-settings-section>.temperature-setting-row+.temperature-autotune-row,#page-settings .temperature-settings-section>.temperature-setting-row+.temperature-setting-row{margin-top:7px}#page-settings .temperature-settings-section-head+.temperature-autotune-row,#page-settings .temperature-settings-section-head+.temperature-setting-row{border-top:1px solid #303944!important}#page-settings .temperature-settings-section>.pid-parameter-grid,#page-settings .temperature-settings-section>.pressure-parameter-grid{margin-top:0}#page-settings .temperature-settings-section .pid-parameter-card{background:#12171c;border-color:#303944}@media (max-width:520px){#page-settings .temperature-settings-section>.temperature-autotune-row,#page-settings .temperature-settings-section>.temperature-setting-row{padding:10px}}#page-maintenance .service-panel{width:100%;max-width:780px;margin:0 auto}#page-maintenance .service-panel-head{display:flex;align-items:flex-start;justify-content:space-between;gap:12px}#page-maintenance .service-subsection-title{margin:14px 3px 7px;padding:0 1px;color:#dce2e9;font-size:var(--zone-font-text)!important;font-weight:var(--ha-font-weight-medium);line-height:1.25}#page-maintenance .service-subsection-title:first-of-type{margin-top:12px}#page-maintenance .service-fields{grid-template-columns:minmax(0,1fr);gap:7px;margin-top:0}#page-maintenance .service-fields .setting,#page-maintenance .service-fields .toggle-setting{min-height:58px;margin:0;padding:10px 12px;border:1px solid #303944;border-radius:11px;background:#12171c}#page-maintenance .service-fields .setting{display:grid;grid-template-columns:minmax(0,1fr) minmax(116px,150px);align-items:center;gap:14px}#page-maintenance .service-fields .toggle-setting{display:grid;grid-template-columns:minmax(0,1fr) auto;align-items:center;gap:14px}#page-maintenance .service-setting-copy,#page-maintenance .toggle-copy{min-width:0;display:grid;gap:3px}#page-maintenance .service-setting-control{min-width:0;display:grid;grid-template-columns:minmax(78px,1fr) auto;align-items:center;gap:7px}#page-maintenance .service-setting-control input,#page-maintenance .service-setting-control select{min-width:0;width:100%}#page-maintenance .service-setting-control>span{min-width:24px;color:#8995a2;font-size:var(--zone-font-small)!important;text-align:left;white-space:nowrap}#page-maintenance .service-status-grid{grid-template-columns:minmax(0,1fr);gap:7px;margin-top:0}#page-maintenance .service-status-grid>div{min-height:48px;padding:9px 11px;border-radius:11px;background:#12171c}#page-maintenance .service-status-wide{grid-column:1}#page-maintenance .service-status-single{max-width:none;margin-left:0}#page-maintenance .service-actions{justify-content:flex-start;margin-top:8px}#page-maintenance .service-actions-danger{padding-top:0;border-top:0}#page-maintenance .service-group-help{margin:-1px 3px 7px;color:#7f8995;font-size:var(--zone-font-small)!important;line-height:1.4}#page-maintenance .service-note{margin-top:8px}#page-maintenance .service-help-panel .service-help-text{display:none!important}#page-maintenance .service-help-panel.help-all-open .service-help-text{display:block!important;animation:settingsHelpReveal .14s ease-out}@media (max-width:520px){#page-maintenance .service-fields .setting,#page-maintenance .service-fields .toggle-setting{grid-template-columns:minmax(0,1fr);gap:8px;padding:10px}#page-maintenance .service-setting-control{grid-template-columns:minmax(0,1fr) auto}#page-maintenance .service-fields .toggle-setting .toggle-control{justify-self:end}}
  
/* Home pressure gauge: clean arc with the reading centered inside it. */
#page-home .pressure-variant-a{
  display:grid!important;
  grid-template-columns:minmax(0,1fr)!important;
  grid-template-rows:minmax(0,1fr)!important;
  place-items:stretch!important;
  padding:3px!important;
}
#page-home .pressure-variant-a .home-pressure-gauge{
  grid-area:1/1;
  width:100%!important;
  height:auto!important;
  min-height:0!important;
  max-height:none!important;
  align-self:stretch!important;
  justify-self:stretch!important;
}
#page-home .pressure-variant-a .pressure-reading-stacked{
  top:calc(50% + 3px)!important;
  left:50%!important;
  gap:4px!important;
  transform:translate(-50%,-50%)!important;
}
#page-home .pressure-variant-a .pressure-reading-stacked strong{
  font-size:38px!important;
  line-height:.86!important;
  font-weight:780;
  color:#f1f5f8;
  text-shadow:0 2px 9px rgba(0,0,0,.34);
}
#page-home .pressure-variant-a .pressure-reading-stacked em{
  color:#8996a3;
  font-size:9px!important;
  line-height:1;
  font-style:normal;
  font-weight:760;
  letter-spacing:.13em;
}
#page-home .pressure-variant-a .home-pressure-backflush{
  position:relative!important;
  grid-area:2/1;
  inset:auto!important;
  width:100%;
  min-height:25px;
  justify-self:stretch;
  align-self:stretch;
  gap:6px;
  padding:6px 0 0;
  color:#96a5b2;
  font-size:10px;
  font-weight:650;
}
#page-home .pressure-variant-a .home-pressure-backflush-icon{
  width:13px;
  height:13px;
  display:inline-grid;
  place-items:center;
  flex:0 0 auto;
  color:#76a9cc;
}
#page-home .pressure-variant-a .home-pressure-backflush-icon svg{
  width:13px;
  height:13px;
  display:block;
  fill:none;
  stroke:currentColor;
  stroke-width:1.8;
  stroke-linecap:round;
  stroke-linejoin:round;
}

/* XDB401 diagnostics: incident-first layout */
#page-diagnostics .diagnostics-sensor-clean{
  margin-top:12px;
  padding:0;
  border:0;
  border-radius:0;
  background:transparent;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-status{
  grid-template-columns:minmax(0,1fr);
  margin-top:0;
  padding:10px 12px;
  border:1px solid #303b46;
  border-left:3px solid #596978;
  border-radius:10px;
  background:#12181e;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-status.ok{
  border-color:#303b46;
  border-left-color:#4bb77f;
  background:#12181e;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-status.warning{
  border-color:#303b46;
  border-left-color:#d4a94d;
  background:#12181e;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-status.critical{
  border-color:#303b46;
  border-left-color:#df6558;
  background:#12181e;
}
#page-diagnostics .xdb-human-status-top{
  display:flex;
  align-items:flex-start;
  justify-content:space-between;
  gap:12px;
}
#page-diagnostics .xdb-human-status-title{
  min-width:0;
  display:grid;
  gap:3px;
}
#page-diagnostics .xdb-human-status-title>span{
  color:#7f8c98;
  font-size:9px;
  font-weight:700;
  letter-spacing:.06em;
  text-transform:uppercase;
}
#page-diagnostics .xdb-human-status-title>b{
  color:#dce4ec;
  font-size:13px;
  font-weight:var(--ha-font-weight-medium);
  line-height:1.25;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-copy>p{
  margin:5px 0 0;
  color:#8996a2;
  font-size:10px;
  line-height:1.4;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-chips{
  margin-top:7px;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-human-chips>span{
  padding:4px 7px;
  background:#0f151a;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-system-summary-compact{
  margin-top:10px;
  padding:8px 1px 0;
  border:0;
  border-top:1px solid #29333d;
  border-radius:0;
  background:transparent;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-system-summary-compact.ok,
#page-diagnostics .diagnostics-sensor-clean .xdb-system-summary-compact.warning,
#page-diagnostics .diagnostics-sensor-clean .xdb-system-summary-compact.critical{
  border-color:#29333d;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-system-summary-compact>span{
  padding:4px 7px;
  background:#0f151a;
}
#page-diagnostics .diagnostics-sensor-clean .diagnostics-primary-label{
  margin-top:15px;
  color:#a8b5c1;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-shot-card-primary{
  margin-top:7px;
  padding:13px 14px;
  border-color:#3a4753;
  background:#141b21;
  box-shadow:inset 3px 0 0 #4e89b5;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-shot-card-primary.clear{
  border-color:#34443d;
  box-shadow:inset 3px 0 0 #4bb77f;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-shot-card-primary.problem{
  border-color:#49383a;
  box-shadow:inset 3px 0 0 #df6558;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-shot-card-primary .xdb-shot-head>div>b{
  font-size:13px;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-shot-fact-wide{
  grid-column:1/-1;
}
#page-diagnostics .diagnostics-sensor-clean .xdb-shot-facts[hidden]{
  display:none;
}
#page-diagnostics .diagnostics-sensor-clean .diagnostics-csv-actions{
  margin-top:7px;
  justify-content:flex-start;
}
#page-diagnostics .diagnostics-sensor-clean .diagnostics-csv-actions .button{
  width:auto;
  min-width:178px;
  padding:8px 11px;
  border-radius:9px;
  font-size:10px!important;
}
#page-diagnostics .diagnostics-sensor-clean>.diagnostics-section-label{
  margin-top:14px;
}

/* XDB401 pressure tab typography aligned with the rest of the dashboard */
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-human-status-title>span{
  font-size:var(--zone-font-small)!important;
  letter-spacing:.035em;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-human-status-title>b{
  font-size:15px!important;
  line-height:1.3;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-human-copy>p{
  font-size:var(--zone-font-small)!important;
  line-height:1.45;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-human-chips>span{
  font-size:var(--zone-font-small)!important;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-system-summary-compact>div>b{
  font-size:14px!important;
  line-height:1.3;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-system-summary-compact>div>p{
  font-size:var(--zone-font-small)!important;
  line-height:1.45;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-system-summary-compact>span{
  font-size:var(--zone-font-small)!important;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-shot-card-primary .xdb-shot-head>div>b{
  font-size:14px!important;
  line-height:1.3;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-shot-card-primary .xdb-shot-head>div>span,
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-shot-card-primary>p{
  font-size:var(--zone-font-small)!important;
  line-height:1.45;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-shot-facts span{
  font-size:var(--zone-font-small)!important;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .xdb-shot-facts b{
  font-size:13px!important;
}
#page-diagnostics [data-diagnostics-panel="pressure"] .diagnostics-csv-actions .button{
  font-size:var(--zone-font-small)!important;
}
@media (max-width:620px){
  #page-diagnostics .xdb-human-status-top{
    flex-direction:column;
    gap:6px;
  }
  #page-diagnostics .diagnostics-sensor-clean .diagnostics-csv-actions .button{
    width:100%;
  }
}

.profile-phase-edit-button{width:34px;min-width:34px;height:34px;padding:0;display:grid;place-items:center;border:1px solid #3d4855;background:#20262e;color:#cbd3dd;font-size:16px;line-height:1}
.profile-phase-edit-button:hover{border-color:#8a633f;background:#2d251e;color:#ffd1aa}
.profile-phase-edit-button.active{border-color:#a87547;background:#3a2b20;color:#ffd1aa;box-shadow:0 0 0 2px rgba(225,139,72,.09)}
.profile-phase-edit-button:disabled{opacity:.4}
.profile-phase-view-strip{display:flex;gap:6px;min-height:48px;margin:0 0 7px;overflow-x:auto;padding:1px 0 4px;scrollbar-width:thin}
.profile-phase-view-item{position:relative;min-width:88px;flex:1 0 88px;padding:8px 9px;border:1px solid #34404b;border-radius:10px;background:linear-gradient(145deg,#171c22,#11161b);overflow:hidden}
.profile-phase-view-item::before{content:"";position:absolute;top:0;left:11px;right:11px;height:2px;border-radius:0 0 999px 999px;background:#d9aa50;opacity:.8}
.profile-phase-view-item.pause::before{background:#8a72ad}
.profile-phase-view-item small,.profile-phase-view-item b{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.profile-phase-view-item small{color:#84909d;font-size:8px}
.profile-phase-view-item b{margin-top:3px;color:#e2e7ed;font-size:11px;font-weight:650}
.profile-phase-view-item.pause{border-color:#443a54;background:linear-gradient(145deg,#1c1822,#131118)}
.profile-phase-view-empty{width:100%;display:grid;place-items:center;min-height:44px;color:#7f8995;font-size:10px}
.profile-phase-view-strip{display:none!important}
#profilePhaseEditorHost:empty{display:none}
.advanced-phase-editor{margin:8px 0 7px;padding:10px;border:1px solid #5e4932;border-radius:12px;background:linear-gradient(145deg,#1d1916,#12161b)}
.advanced-phase-editor[hidden]{display:none!important}
.advanced-phase-toolbar{display:flex;align-items:center;justify-content:space-between;gap:8px;margin-bottom:8px}
.advanced-phase-toolbar-main{min-width:0}
.advanced-phase-toolbar-main b{display:block;color:#f0f2f5;font-size:13px}
.advanced-phase-toolbar-main small{display:block;margin-top:2px;color:#8d97a3;font-size:9px}
.advanced-phase-toolbar-actions{display:flex;gap:6px;flex:0 0 auto}
.advanced-phase-toolbar-actions button{min-height:31px;padding:6px 9px;font-size:10px}
.advanced-phase-done{border:1px solid #896136!important;background:#e18b48!important;color:#1a120d!important}
.advanced-phase-list{display:flex!important;gap:6px;margin:0!important;padding:1px 0 5px;overflow-x:auto;scrollbar-width:thin}
.advanced-phase-card{position:relative;min-width:104px;flex:1 0 104px;padding:8px 9px;border:1px solid #35404c;border-radius:10px;background:#151a20;color:#dce2e9;text-align:left;box-shadow:none}
.advanced-phase-card:hover{border-color:#586574;background:#1c2229}
.advanced-phase-card.selected{border-color:#d6a94d;background:#2b2519;box-shadow:inset 0 0 0 1px rgba(214,169,77,.12)}
.advanced-phase-card.pause{border-color:#4b405b;background:#19161e}
.advanced-phase-card.preinfusion{border-color:#4e725f;background:#16221c}
.advanced-phase-card small,.advanced-phase-card b{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.advanced-phase-card small{color:#87929f;font-size:8px}
.advanced-phase-card b{margin-top:3px;font-size:10px}
.advanced-phase-card-index{position:absolute;top:5px;right:6px;color:#687481;font-size:8px}
.advanced-phase-add{display:flex;gap:6px;margin-top:7px;flex-wrap:wrap}
.advanced-phase-add button{min-height:31px;padding:6px 9px;border:1px solid #3b4652;background:#20262e;color:#d7dde5;font-size:10px}
.advanced-phase-add button:first-child{border-color:#6e5832;color:#f0ce79}
.advanced-phase-detail{margin-top:8px;padding:9px;border:1px solid #303a45;border-radius:10px;background:#10151a}
.advanced-phase-detail-head{display:flex;align-items:center;justify-content:space-between;gap:8px;margin-bottom:8px}
.advanced-phase-detail-head b{font-size:11px}
.advanced-phase-type-buttons{display:flex;gap:5px}
.advanced-phase-type-buttons button{min-height:28px;padding:5px 8px;font-size:9px}
.advanced-phase-type-buttons button.active{border:1px solid #896d35;background:#342b19;color:#f0cf77}
.advanced-phase-fields{display:grid;grid-template-columns:repeat(4,minmax(90px,1fr));gap:7px}
.advanced-phase-field{min-width:0;display:grid;gap:4px}
.advanced-phase-field span{color:#8994a1;font-size:8px}
.advanced-phase-field input{width:100%;min-width:0;min-height:32px;padding:6px 7px;border:1px solid #34404b;border-radius:8px;background:#0d1216;color:#e8edf2;font-size:10px}
.advanced-phase-detail-actions{display:flex;gap:5px;margin-top:8px;flex-wrap:wrap}
.advanced-phase-detail-actions button{min-height:29px;padding:5px 8px;font-size:9px}
.advanced-phase-detail-actions .danger{margin-left:auto}
.shot-chart-card.phase-editing #chart{touch-action:none;user-select:none}
.shot-chart-card.phase-editing .profile-phase-view-strip{display:none}
.shot-chart-card:not(.phase-editing) #profilePhaseEditorHost{display:none}
@media(max-width:620px){
  .advanced-phase-toolbar{align-items:flex-start}
  .advanced-phase-toolbar-actions{flex-wrap:wrap;justify-content:flex-end}
  .advanced-phase-fields{grid-template-columns:repeat(2,minmax(0,1fr))}
}
@media(max-width:430px){
  .advanced-phase-toolbar{display:block}
  .advanced-phase-toolbar-actions{justify-content:flex-start;margin-top:7px}
  .advanced-phase-fields{grid-template-columns:minmax(0,1fr)}
  .advanced-phase-detail-actions .danger{margin-left:0}
}
</style>
  <style>
    .user-profile-actions { grid-template-columns: 34px; }
    .user-profile-actions button { width:34px; min-width:34px; }
    .user-profile-actions .user-profile-delete { color:#e98282; font-size:13px; }
    .user-profile-actions .user-profile-delete:hover:not(:disabled) { background:#3b2227; color:#ffaaaa; }
    .home-archive-head-actions{display:flex;align-items:center;gap:6px}
    .home-archive-head-actions button{min-width:38px;height:38px;padding:0;display:grid;place-items:center}
    .home-archive-list{align-content:start;grid-auto-rows:max-content}
    .archive-row-card{display:grid;grid-template-columns:minmax(0,1fr) 34px;align-items:stretch;min-height:76px;overflow:hidden;margin-bottom:5px;border:1px solid #303a46;border-radius:9px;background:#171c22}
    .archive-row-card.active{border-color:#9b673e;background:#2d241c;box-shadow:inset 0 0 0 1px rgba(225,139,72,.08)}
    .archive-row-open{min-width:0;display:grid;grid-template-columns:minmax(0,1fr);gap:0;padding:9px 10px;border:0;border-radius:0;background:transparent;color:#dce2e9;text-align:left;box-shadow:none}
    .archive-row-open:hover:not(:disabled){background:#20262e}
    .archive-row-card.active .archive-row-open:hover:not(:disabled){background:#35291f}
    .archive-row-profile{overflow:hidden;font-size:var(--zone-font-text,14px);font-weight:760;line-height:1.25;text-overflow:ellipsis;white-space:nowrap}
    .archive-row-date{overflow:hidden;margin-top:4px;color:#778491;font-size:var(--zone-font-small,12px);font-weight:570;line-height:1.25;text-overflow:ellipsis;white-space:nowrap}
    .archive-row-metrics{overflow:hidden;margin-top:4px;color:#9aa4b0;font-size:var(--zone-font-small,12px);font-weight:600;line-height:1.25;text-overflow:ellipsis;white-space:nowrap}
    .archive-row-delete{align-self:stretch;width:34px;min-width:34px;padding:0;border:0;border-left:1px solid #303a46;border-radius:0;background:transparent;color:#e98282;font-size:13px;box-shadow:none}
    .archive-row-delete:hover:not(:disabled){background:#3b2227;color:#ffaaaa}
    .user-profile-apply .user-profile-date { margin-top:3px; color:#6f7c89; font-size:7px; }
    .lab-grid{display:grid;grid-template-columns:minmax(300px,.82fr) minmax(360px,1.18fr);gap:12px;align-items:start}
    .lab-overview,.lab-records{min-width:0}
    .lab-collection-row{display:grid;grid-template-columns:minmax(0,1fr) auto;align-items:center;gap:14px;padding:12px;border:1px solid #303a46;border-radius:12px;background:#12171c}
    .lab-collection-copy b,.lab-record-head b{display:block;color:#dce3ea;font-size:14px}
    .lab-collection-copy small,.lab-record-head small{display:block;margin-top:3px;color:#7f8b97;font-size:11px;line-height:1.35}
    .lab-readiness{margin-top:11px;padding:13px;border:1px solid #34404b;border-left:4px solid #677584;border-radius:12px;background:#11171c}
    .lab-readiness.collecting{border-left-color:#5599cf}.lab-readiness.prototype{border-left-color:#d4a94d}.lab-readiness.high{border-left-color:#4bb77f}.lab-readiness.problem{border-left-color:#df6558}
    .lab-readiness-head{display:flex;align-items:flex-start;justify-content:space-between;gap:12px}
    .lab-readiness-head small{display:block;color:#7e8a96;font-size:10px;font-weight:700;letter-spacing:.05em;text-transform:uppercase}
    .lab-readiness-head b{display:block;margin-top:3px;color:#dce4eb;font-size:17px}
    .lab-readiness-score{flex:0 0 auto;color:#aeb9c4;font-size:20px;font-weight:800;font-variant-numeric:tabular-nums}
    .lab-progress{height:7px;margin-top:11px;overflow:hidden;border-radius:999px;background:#252d35}
    .lab-progress>i{display:block;width:0;height:100%;border-radius:inherit;background:#667887;transition:width .25s ease}
    .lab-readiness.collecting .lab-progress>i{background:#5599cf}.lab-readiness.prototype .lab-progress>i{background:#d4a94d}.lab-readiness.high .lab-progress>i{background:#4bb77f}.lab-readiness.problem .lab-progress>i{background:#df6558}
    .lab-readiness p{margin:9px 0 0;color:#8e9aa5;font-size:11px;line-height:1.45}
    .lab-metrics{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:7px;margin-top:10px}
    .lab-metric{min-width:0;padding:10px;border:1px solid #303a46;border-radius:10px;background:#12171c}
    .lab-metric span,.lab-storage-label{display:block;color:#788591;font-size:10px}
    .lab-metric b{display:block;margin-top:4px;color:#d5dde5;font-size:16px;font-variant-numeric:tabular-nums}
    .lab-storage{margin-top:10px;padding:11px 12px;border:1px solid #303a46;border-radius:11px;background:#12171c}
    .lab-storage-top{display:flex;align-items:center;justify-content:space-between;gap:10px}
    .lab-storage-top b{color:#cfd7df;font-size:12px;font-variant-numeric:tabular-nums}
    .lab-storage-bar{height:6px;margin-top:8px;overflow:hidden;border-radius:999px;background:#252d35}
    .lab-storage-bar>i{display:block;width:0;height:100%;border-radius:inherit;background:#6f8395}
    .lab-actions{display:flex;flex-wrap:wrap;gap:8px;margin-top:11px}.lab-actions>*{flex:1 1 150px}
    .lab-export-progress{min-height:17px;margin-top:8px;color:#7f8b97;font-size:10px;line-height:1.4}
    .lab-records-head{display:flex;align-items:center;justify-content:space-between;gap:10px;margin-bottom:9px}
    .lab-records-head button{min-width:36px;height:36px;padding:0}
    .lab-record-list{display:grid;gap:7px;max-height:620px;overflow:auto}
    .lab-record{display:grid;grid-template-columns:10px minmax(0,1fr) auto;align-items:center;gap:10px;padding:10px;border:1px solid #303a46;border-radius:11px;background:#12171c}
    .lab-record-dot{width:9px;height:9px;border-radius:50%;background:#677584}.lab-record.clean .lab-record-dot{background:#4bb77f}.lab-record.partial .lab-record-dot{background:#d4a94d}.lab-record.rejected .lab-record-dot{background:#df6558}
    .lab-record-copy{min-width:0}.lab-record-title{display:flex;align-items:center;gap:7px;min-width:0}.lab-record-title b{overflow:hidden;color:#d8e0e7;font-size:12px;text-overflow:ellipsis;white-space:nowrap}.lab-record-title span{flex:0 0 auto;padding:3px 6px;border-radius:999px;background:#252d35;color:#9ca8b3;font-size:8px;font-weight:750;text-transform:uppercase}
    .lab-record.clean .lab-record-title span{background:#183326;color:#81dca6}.lab-record.partial .lab-record-title span{background:#352e1b;color:#e4c56f}.lab-record.rejected .lab-record-title span{background:#372126;color:#ed918a}
    .lab-record-copy small{display:block;margin-top:4px;overflow:hidden;color:#778490;font-size:9px;line-height:1.3;text-overflow:ellipsis;white-space:nowrap}
    .lab-pin{width:34px;min-width:34px;height:34px;padding:0;border:1px solid #38434e;background:#1c232a;color:#74808c;font-size:16px}.lab-pin.active{border-color:#725f31;background:#312918;color:#f0c864}
    .lab-empty{padding:28px 14px;border:1px dashed #3a4651;border-radius:11px;color:#788591;text-align:center}
    @media(max-width:820px){.lab-grid{grid-template-columns:minmax(0,1fr)}.lab-record-list{max-height:none}}
    @media(max-width:430px){.lab-metrics{grid-template-columns:minmax(0,1fr)}.lab-actions>*{flex-basis:100%}}
    .profile-library .home-profile-buttons button:last-child{grid-column:auto!important}
    .profile-library .home-profile-buttons button.custom-unavailable{display:grid;gap:1px;align-content:center}
    .profile-library .home-profile-buttons button.custom-unavailable small{display:block;color:#697480;font-size:7px;font-weight:650;line-height:1}
    .shot-line-quick-grid{grid-template-columns:minmax(0,1fr)!important}
    .shot-line-quick-row{grid-template-columns:minmax(92px,.55fr) 48px minmax(110px,1fr) minmax(110px,1fr)}
    .shot-line-quick-actions{min-width:0;flex-wrap:wrap}
    @media (max-width:680px){
      .shot-line-quick-head{display:block}
      .shot-line-quick-actions{justify-content:flex-start;margin-top:7px}
    }
    @media (max-width:520px){
      .shot-line-quick-row{grid-template-columns:minmax(78px,.55fr) 42px minmax(0,1fr)}
      .shot-line-quick-control:last-child{grid-column:3}
    }
  </style>
</head>
<body class="sidebar-collapsed" style="visibility:hidden" onload="this.style.visibility=''">
  <div class="shell">
    <aside id="sidebar" class="sidebar">
      <div class="side-head">
        <button class="menu-toggle" type="button" onclick="toggleSidebar()" aria-label="Свернуть меню">
          <svg viewBox="0 0 24 24" aria-hidden="true">
            <path d="M4 7h16"/><path d="M4 12h16"/><path d="M4 17h16"/>
          </svg>
        </button>
        <div class="side-brand">Rancilio Silvia<small>Локальная панель</small></div>
      </div>
      <nav class="side-nav" aria-label="Разделы">
        <button class="nav-item active" data-page="home" title="Главная">
          <span class="nav-icon" aria-hidden="true">
            <svg viewBox="0 0 24 24">
              <path d="m3 11 9-7 9 7"/><path d="M5.5 10v10h13V10"/><path d="M9.5 20v-6h5v6"/>
            </svg>
          </span>
          <span class="nav-label">Главная</span>
        </button>
        <button class="nav-item" data-page="lab" title="Лаборатория">
          <span class="nav-icon" aria-hidden="true">
            <svg viewBox="0 0 24 24">
              <path d="M9 3h6"/><path d="M10 3v6l-5 9a2 2 0 0 0 1.8 3h10.4a2 2 0 0 0 1.8-3l-5-9V3"/>
              <path d="M7.5 15h9"/><circle cx="10" cy="17.5" r=".7"/><circle cx="14" cy="13" r=".7"/>
            </svg>
          </span>
          <span class="nav-label">Лаборатория</span>
        </button>
        <button class="nav-item" data-page="settings" title="Настройки">
          <span class="nav-icon" aria-hidden="true">
            <svg viewBox="0 0 24 24">
              <path d="M4 7h8"/><path d="M16 7h4"/><circle cx="14" cy="7" r="2"/>
              <path d="M4 17h4"/><path d="M12 17h8"/><circle cx="10" cy="17" r="2"/>
            </svg>
          </span>
          <span class="nav-label">Настройки</span>
        </button>
        <button class="nav-item" data-page="maintenance" title="Обслуживание">
          <span class="nav-icon" aria-hidden="true">
            <svg viewBox="0 0 24 24">
              <path d="M14.5 6.5a4 4 0 0 0-5-5l2.2 2.2-2.8 2.8-2.2-2.2a4 4 0 0 0 5 5L19 16.6a1.7 1.7 0 0 1-2.4 2.4l-7.3-7.3"/>
              <path d="m5 19 3.5-3.5"/>
            </svg>
          </span>
          <span class="nav-label">Обслуживание</span>
        </button>
        <button class="nav-item" data-page="diagnostics" title="Диагностика">
          <span class="nav-icon" aria-hidden="true">
            <svg viewBox="0 0 24 24">
              <path d="M3 12h4l2-5 4 10 2-5h6"/><path d="M4 4h16v16H4z"/>
            </svg>
          </span>
          <span class="nav-label">Диагностика</span>
        </button>
      </nav>
      <div class="side-footer">Rancilio Silvia · ESP32-S3</div>
    </aside>
    <div class="backdrop" onclick="closeSidebar()"></div>

    <main class="content">
      <header>
        <button class="mobile-menu" type="button" onclick="toggleSidebar()" aria-label="Открыть меню">☰</button>
        <div class="brand">Rancilio Silvia</div>
        <div class="header-spacer"></div>
        <div id="headerLanguageSwitch" class="header-language-switch" role="group" aria-label="Язык интерфейса">
          <button id="languageRuButton" type="button" onclick="setLanguage('ru')" aria-label="Русский язык">RU</button>
          <button id="languageEnButton" type="button" onclick="setLanguage('en')" aria-label="Английский язык">EN</button>
        </div>
        <button id="fontSettingsButton" class="home-layout-header-button typography-header-button" type="button"
          onclick="toggleTypographyPanel()" aria-label="Настроить шрифты" title="Настроить шрифты" aria-pressed="false">
          <span aria-hidden="true">Aa</span>
        </button>
        <button id="homeLayoutEditButton" class="home-layout-header-button" type="button"
          onclick="toggleHomeDashboardEdit()" aria-label="Настроить экран" title="Настроить экран" aria-pressed="false">
          <svg viewBox="0 0 24 24" aria-hidden="true">
            <path d="M4 7h9"/><path d="M17 7h3"/><circle cx="15" cy="7" r="2"/>
            <path d="M4 17h3"/><path d="M11 17h9"/><circle cx="9" cy="17" r="2"/>
          </svg>
        </button>
        <div id="online" class="online">подключение…</div>
      </header>

      <section id="page-home" class="page active">
        <div id="homeFontTools" class="home-font-tools" hidden>
          <div class="home-font-head">
            <div>
              <b>Независимые размеры шрифтов</b>
              <small>Каждый блок меняется отдельно. Размеры указаны прямо в пикселях.</small>
            </div>
            <button class="small ha-typography-preset-button" type="button"
              onclick="resetTypographySettings()" title="Вернуть размеры Home Assistant">
              ↺ Сбросить всё
            </button>
          </div>

          <div class="home-font-independent-note">
            Изменение одного окна больше не влияет на остальные окна. Текущий набор сохранён как стандартный.
          </div>

          <div class="home-font-windows">
            <details class="home-font-window" data-font-window="sidebar" open>
              <summary>
                <span>
                  <b>Левое меню</b>
                  <small>Настраивается независимо</small>
                </span>
                <output id="fontSidebarSummary">14 / 16 / 18 / 10 px</output>
              </summary>

              <div class="home-font-window-controls">
                <label class="home-font-control">
                  <span><b>Текст</b><output id="fontSidebarTextValue">14 px</output></span>
                  <input id="fontSidebarTextRange" type="range" min="9" max="22" step="1" value="14"
                    oninput="updateTypographyZoneField('sidebar','text',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Заголовок</b><output id="fontSidebarTitleValue">16 px</output></span>
                  <input id="fontSidebarTitleRange" type="range" min="11" max="32" step="1" value="16"
                    oninput="updateTypographyZoneField('sidebar','title',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Значения</b><output id="fontSidebarValueValue">18 px</output></span>
                  <input id="fontSidebarValueRange" type="range" min="11" max="52" step="1" value="18"
                    oninput="updateTypographyZoneField('sidebar','value',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>График / мелкие подписи</b><output id="fontSidebarChartValue">10 px</output></span>
                  <input id="fontSidebarChartRange" type="range" min="8" max="20" step="1" value="10"
                    oninput="updateTypographyZoneField('sidebar','chart',this.value)">
                </label>
              </div>

              <div class="home-font-window-actions">
                <button class="small" type="button" onclick="resetTypographyZone('sidebar')">↺ Сбросить это окно</button>
              </div>
            </details>

            <details class="home-font-window" data-font-window="machine">
              <summary>
                <span>
                  <b>Управление машиной</b>
                  <small>Настраивается независимо</small>
                </span>
                <output id="fontMachineSummary">14 / 16 / 14 / 10 px</output>
              </summary>

              <div class="home-font-window-controls">
                <label class="home-font-control">
                  <span><b>Текст</b><output id="fontMachineTextValue">14 px</output></span>
                  <input id="fontMachineTextRange" type="range" min="9" max="22" step="1" value="14"
                    oninput="updateTypographyZoneField('machine','text',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Заголовок</b><output id="fontMachineTitleValue">16 px</output></span>
                  <input id="fontMachineTitleRange" type="range" min="11" max="32" step="1" value="16"
                    oninput="updateTypographyZoneField('machine','title',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Значения</b><output id="fontMachineValueValue">14 px</output></span>
                  <input id="fontMachineValueRange" type="range" min="11" max="52" step="1" value="14"
                    oninput="updateTypographyZoneField('machine','value',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>График / мелкие подписи</b><output id="fontMachineChartValue">10 px</output></span>
                  <input id="fontMachineChartRange" type="range" min="8" max="20" step="1" value="10"
                    oninput="updateTypographyZoneField('machine','chart',this.value)">
                </label>
              </div>

              <div class="home-font-window-actions">
                <button class="small" type="button" onclick="resetTypographyZone('machine')">↺ Сбросить это окно</button>
              </div>
            </details>

            <details class="home-font-window" data-font-window="metrics">
              <summary>
                <span>
                  <b>Показатели</b>
                  <small>Настраивается независимо</small>
                </span>
                <output id="fontMetricsSummary">13 / 16 / 25 / 10 px</output>
              </summary>

              <div class="home-font-window-controls">
                <label class="home-font-control">
                  <span><b>Текст</b><output id="fontMetricsTextValue">13 px</output></span>
                  <input id="fontMetricsTextRange" type="range" min="9" max="22" step="1" value="13"
                    oninput="updateTypographyZoneField('metrics','text',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Заголовок</b><output id="fontMetricsTitleValue">16 px</output></span>
                  <input id="fontMetricsTitleRange" type="range" min="11" max="32" step="1" value="16"
                    oninput="updateTypographyZoneField('metrics','title',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Значения</b><output id="fontMetricsValueValue">25 px</output></span>
                  <input id="fontMetricsValueRange" type="range" min="11" max="52" step="1" value="25"
                    oninput="updateTypographyZoneField('metrics','value',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>График / мелкие подписи</b><output id="fontMetricsChartValue">10 px</output></span>
                  <input id="fontMetricsChartRange" type="range" min="8" max="20" step="1" value="10"
                    oninput="updateTypographyZoneField('metrics','chart',this.value)">
                </label>
              </div>

              <div class="home-font-window-actions">
                <button class="small" type="button" onclick="resetTypographyZone('metrics')">↺ Сбросить это окно</button>
              </div>
            </details>

            <details class="home-font-window" data-font-window="temperature">
              <summary>
                <span>
                  <b>Температурный график</b>
                  <small>Настраивается независимо</small>
                </span>
                <output id="fontTemperatureSummary">12 / 20 / 16 / 10 px</output>
              </summary>

              <div class="home-font-window-controls">
                <label class="home-font-control">
                  <span><b>Текст</b><output id="fontTemperatureTextValue">12 px</output></span>
                  <input id="fontTemperatureTextRange" type="range" min="9" max="22" step="1" value="12"
                    oninput="updateTypographyZoneField('temperature','text',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Заголовок</b><output id="fontTemperatureTitleValue">20 px</output></span>
                  <input id="fontTemperatureTitleRange" type="range" min="11" max="32" step="1" value="20"
                    oninput="updateTypographyZoneField('temperature','title',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Значения</b><output id="fontTemperatureValueValue">16 px</output></span>
                  <input id="fontTemperatureValueRange" type="range" min="11" max="52" step="1" value="16"
                    oninput="updateTypographyZoneField('temperature','value',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>График / мелкие подписи</b><output id="fontTemperatureChartValue">10 px</output></span>
                  <input id="fontTemperatureChartRange" type="range" min="8" max="20" step="1" value="10"
                    oninput="updateTypographyZoneField('temperature','chart',this.value)">
                </label>
              </div>

              <div class="home-font-window-actions">
                <button class="small" type="button" onclick="resetTypographyZone('temperature')">↺ Сбросить это окно</button>
              </div>
            </details>

            <details class="home-font-window" data-font-window="shot">
              <summary>
                <span>
                  <b>График пролива</b>
                  <small>Настраивается независимо</small>
                </span>
                <output id="fontShotSummary">12 / 16 / 16 / 12 px</output>
              </summary>

              <div class="home-font-window-controls">
                <label class="home-font-control">
                  <span><b>Текст</b><output id="fontShotTextValue">12 px</output></span>
                  <input id="fontShotTextRange" type="range" min="9" max="22" step="1" value="12"
                    oninput="updateTypographyZoneField('shot','text',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Заголовок</b><output id="fontShotTitleValue">16 px</output></span>
                  <input id="fontShotTitleRange" type="range" min="11" max="32" step="1" value="16"
                    oninput="updateTypographyZoneField('shot','title',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Значения</b><output id="fontShotValueValue">16 px</output></span>
                  <input id="fontShotValueRange" type="range" min="11" max="52" step="1" value="16"
                    oninput="updateTypographyZoneField('shot','value',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>График / мелкие подписи</b><output id="fontShotChartValue">12 px</output></span>
                  <input id="fontShotChartRange" type="range" min="8" max="20" step="1" value="12"
                    oninput="updateTypographyZoneField('shot','chart',this.value)">
                </label>
              </div>

              <div class="shot-line-settings">
                <div class="shot-line-settings-head">
                  <span>
                    <b>Линии графика</b>
                    <small>Толщина и прозрачность</small>
                  </span>
                  <button class="small" type="button" onclick="resetShotLineSettings()">↺ Линии по умолчанию</button>
                </div>

                <div class="shot-line-settings-grid">
                  <div class="shot-line-setting-row">
                    <div class="shot-line-setting-name">
                      <i class="dashed" style="color:#ffd166"></i>
                      <b>Профиль</b>
                    </div>
                    <label class="shot-line-control">
                      <span>Толщина <output id="shotLineProfileWidthValue">2.6 px</output></span>
                      <input id="shotLineProfileWidth" type="range" min="0.5" max="5" step="0.1" value="2.6"
                        oninput="updateShotLineSetting('profile','width',this.value)">
                    </label>
                    <label class="shot-line-control">
                      <span>Прозрачность <output id="shotLineProfileTransparencyValue">0%</output></span>
                      <input id="shotLineProfileTransparency" type="range" min="0" max="90" step="5" value="0"
                        oninput="updateShotLineSetting('profile','transparency',this.value)">
                    </label>
                  </div>

                  <div class="shot-line-setting-row">
                    <div class="shot-line-setting-name">
                      <i style="color:#54a9ff"></i>
                      <b>Давление</b>
                    </div>
                    <label class="shot-line-control">
                      <span>Толщина <output id="shotLinePressureWidthValue">2.2 px</output></span>
                      <input id="shotLinePressureWidth" type="range" min="0.5" max="5" step="0.1" value="2.2"
                        oninput="updateShotLineSetting('pressure','width',this.value)">
                    </label>
                    <label class="shot-line-control">
                      <span>Прозрачность <output id="shotLinePressureTransparencyValue">0%</output></span>
                      <input id="shotLinePressureTransparency" type="range" min="0" max="90" step="5" value="0"
                        oninput="updateShotLineSetting('pressure','transparency',this.value)">
                    </label>
                  </div>

                  <div class="shot-line-setting-row">
                    <div class="shot-line-setting-name">
                      <i style="color:#4bd18b"></i>
                      <b>Поток</b>
                    </div>
                    <label class="shot-line-control">
                      <span>Толщина <output id="shotLineFlowWidthValue">1.8 px</output></span>
                      <input id="shotLineFlowWidth" type="range" min="0.5" max="5" step="0.1" value="1.8"
                        oninput="updateShotLineSetting('flow','width',this.value)">
                    </label>
                    <label class="shot-line-control">
                      <span>Прозрачность <output id="shotLineFlowTransparencyValue">0%</output></span>
                      <input id="shotLineFlowTransparency" type="range" min="0" max="90" step="5" value="0"
                        oninput="updateShotLineSetting('flow','transparency',this.value)">
                    </label>
                  </div>

                  <div class="shot-line-setting-row">
                    <div class="shot-line-setting-name">
                      <i style="color:#9a72ef"></i>
                      <b>Помпа</b>
                    </div>
                    <label class="shot-line-control">
                      <span>Толщина <output id="shotLinePumpWidthValue">1.4 px</output></span>
                      <input id="shotLinePumpWidth" type="range" min="0.5" max="5" step="0.1" value="1.4"
                        oninput="updateShotLineSetting('pump','width',this.value)">
                    </label>
                    <label class="shot-line-control">
                      <span>Прозрачность <output id="shotLinePumpTransparencyValue">65%</output></span>
                      <input id="shotLinePumpTransparency" type="range" min="0" max="90" step="5" value="65"
                        oninput="updateShotLineSetting('pump','transparency',this.value)">
                    </label>
                  </div>
                </div>
              </div>

              <div class="home-font-window-actions">
                <button class="small" type="button" onclick="resetTypographyZone('shot')">↺ Сбросить это окно</button>
              </div>
            </details>

            <details class="home-font-window" data-font-window="profiles">
              <summary>
                <span>
                  <b>Профили</b>
                  <small>Настраивается независимо</small>
                </span>
                <output id="fontProfilesSummary">14 / 16 / 14 / 10 px</output>
              </summary>

              <div class="home-font-window-controls">
                <label class="home-font-control">
                  <span><b>Текст</b><output id="fontProfilesTextValue">14 px</output></span>
                  <input id="fontProfilesTextRange" type="range" min="9" max="22" step="1" value="14"
                    oninput="updateTypographyZoneField('profiles','text',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Заголовок</b><output id="fontProfilesTitleValue">16 px</output></span>
                  <input id="fontProfilesTitleRange" type="range" min="11" max="32" step="1" value="16"
                    oninput="updateTypographyZoneField('profiles','title',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>Значения</b><output id="fontProfilesValueValue">14 px</output></span>
                  <input id="fontProfilesValueRange" type="range" min="11" max="52" step="1" value="14"
                    oninput="updateTypographyZoneField('profiles','value',this.value)">
                </label>

                <label class="home-font-control">
                  <span><b>График / мелкие подписи</b><output id="fontProfilesChartValue">10 px</output></span>
                  <input id="fontProfilesChartRange" type="range" min="8" max="20" step="1" value="10"
                    oninput="updateTypographyZoneField('profiles','chart',this.value)">
                </label>
              </div>

              <div class="home-font-window-actions">
                <button class="small" type="button" onclick="resetTypographyZone('profiles')">↺ Сбросить это окно</button>
              </div>
            </details>
          </div>

          <div class="home-font-section-title home-font-transfer-title">
            <b>Перенос настроек</b>
            <small>Скопируйте строку и отправьте её мне — эти размеры можно сделать значениями по умолчанию</small>
          </div>

          <textarea id="fontSettingsTransfer" class="home-font-transfer" rows="4"
            spellcheck="false" aria-label="Настройки шрифтов"></textarea>

          <div class="home-font-transfer-actions">
            <button class="small primary" type="button" onclick="copyTypographySettings()">Копировать настройки</button>
            <button class="small" type="button" onclick="applyTypographySettingsFromText()">Применить из поля</button>
          </div>

          <div id="fontSettingsSummary" class="home-font-summary">
            Roboto / Noto · независимые размеры в px
          </div>
        </div>

        <div id="homeLayoutTools" class="home-layout-tools" hidden>
          <div id="homeLayoutHint" class="home-layout-hint">Перетаскивайте блоки за ⠿ или скрывайте кнопкой справа</div>
          <div class="home-layout-actions">
            <button id="homeLayoutResetButton" class="small" type="button" onclick="resetHomeDashboardLayout()">↺ Сбросить</button>
          </div>

          <div id="machineIconSizeTools" class="machine-icon-size-tools">
            <label for="machineActionIconSize">
              <span>Иконки Пролив / Вода / Пар</span>
              <output id="machineActionIconSizeValue">48 px</output>
              <input id="machineActionIconSize" type="range" min="18" max="48" step="1" value="48"
                oninput="setMachineActionIconSize(this.value)">
            </label>

            <label for="machinePowerButtonSize">
              <span>Круглая кнопка питания</span>
              <output id="machinePowerButtonSizeValue">52 px</output>
              <input id="machinePowerButtonSize" type="range" min="36" max="64" step="1" value="52"
                oninput="setMachinePowerButtonSize(this.value)">
            </label>

            <label for="homeSectionGap">
              <span>Отступ между верхним и нижним блоками</span>
              <output id="homeSectionGapValue">18 px</output>
              <input id="homeSectionGap" type="range" min="8" max="40" step="1" value="18"
                oninput="setHomeSectionGap(this.value)">
            </label>

            <label for="homeTemperatureCardHeight">
              <span>Высота температурного графика</span>
              <output id="homeTemperatureCardHeightValue">165 px</output>
              <input id="homeTemperatureCardHeight" type="range"
                min="145" max="260" step="1" value="165"
                oninput="setHomeTemperatureCardHeight(this.value)">
            </label>

            <label for="homeTopColumnSplit">
              <span>Ширина управление / давление / статистика / температура</span>
              <output id="homeTopColumnSplitValue">38 / 18 / 18 / 26</output>
              <input id="homeTopColumnSplit" type="range"
                min="34" max="48" step="1" value="38"
                oninput="setHomeTopColumnSplit(this.value)">
            </label>

            <button class="small" type="button" onclick="resetMachineControlSizing()">↺ Размеры по умолчанию</button>
          </div>

          <div id="homeHiddenBlocks" class="home-hidden-blocks" hidden>
            <span>Скрытые блоки</span>
            <div id="homeHiddenBlockList" class="home-hidden-block-list"></div>
          </div>
        </div>
        <div class="home-overview">
          <div class="home-top-grid">
            <div class="machine-panel">
              <div class="card machine-card machine-card-restored machine-control-card">
                <div class="machine-top-strip">
                  <div class="machine-power-tile">
                    <button id="powerButton" class="power-inline off" type="button"
                      onclick="machinePower()" aria-label="Питание">
                      <span class="power-symbol" aria-hidden="true">⏻</span>
                      <span id="powerButtonLabel" class="machine-power-label">Выключено</span>
                    </button>
                  </div>

                  <div class="machine-status-banner machine-restored-status">
                    <div class="machine-status-copy">
                      <b id="shotStatus" data-raw-value="Ожидание">Ожидание</b>
                      <span id="remaining" hidden>—</span>

                      <div id="machineAutoOff" class="machine-auto-off"
                        hidden title="Автоотключение">
                        <span class="machine-auto-off-icon" aria-hidden="true">◷</span>
                        <b id="machineAutoOffText">—</b>
                      </div>
                    </div>



                    <div id="machineWaterStatus"
                      class="machine-water-status machine-water-indicator mascot-water-slot unknown"
                      title="Состояние бака воды" role="status">
                      <div id="machineMascot" class="machine-mascot state-off"
                        role="img" aria-label="Кофемашина выключена" title="Выключена">
                        <span class="machine-mascot-eye" aria-hidden="true">
                          <i class="machine-mascot-pupil"></i>
                        </span>
                        <span class="machine-mascot-eye" aria-hidden="true">
                          <i class="machine-mascot-pupil"></i>
                        </span>
                        <span class="machine-mascot-mouth" aria-hidden="true"></span>
                        <span class="machine-mascot-steam" aria-hidden="true">
                          <i></i><i></i><i></i>
                        </span>
                      </div>
                      <span id="machineMascotWaterAlert"
                        class="machine-mascot-water-alert" aria-hidden="true">?</span>
                      <span id="machineWaterIcon" class="machine-water-result"
                        aria-hidden="true">?</span>
                      <b id="machineWaterText" class="machine-water-label">Нет данных</b>
                    </div>

                    <div class="machine-status-hidden" aria-hidden="true">
                      <div class="machine-control-title">Rancilio Silvia</div>
                      <strong id="machineState" class="machine-state">Выключена</strong>

                      <span id="backflushCard">
                        <span id="backflushMain">—</span>
                        <span id="backflushCount">—</span>
                        <span id="backflushLimit">—</span>
                      </span>
                    </div>
                  </div>
                </div>

                <span id="temp" hidden>—</span>
                <span id="tempTarget" hidden>—</span>
                <b id="profile" hidden>—</b>

                <div class="machine-actions-dock">
                  <div class="quick-actions machine-quick-actions">
                    <button id="shotAction" class="quick-action" type="button"
                      onclick="toggleShotAction()" aria-label="Пролив" title="Пролив">
                      <span class="quick-icon" aria-hidden="true">
                        <svg viewBox="0 0 32 32">
                          <path d="M7 11h15v7a7 7 0 0 1-7 7h-1a7 7 0 0 1-7-7v-7Z"/>
                          <path d="M22 14h2.2a3.3 3.3 0 0 1 0 6.6H22"/>
                          <path d="M10 8c0-1.5 1.4-2.1 1.4-3.6M16 8c0-1.5 1.4-2.1 1.4-3.6"/>
                        </svg>
                      </span>
                      <span class="machine-action-copy">
                        <b id="shotActionTitle">Пролив</b>
                        <small id="shotActionState">Машина выключена</small>
                      </span>
                    </button>

                    <button id="waterAction" class="quick-action water" type="button"
                      onclick="toggleHotWater()" aria-label="Вода" title="Вода">
                      <span class="quick-icon" aria-hidden="true">
                        <svg viewBox="0 0 32 32">
                          <path d="M16 4s8 8.7 8 15a8 8 0 1 1-16 0c0-6.3 8-15 8-15Z"/>
                          <path d="M12 20.5c.9 1.7 2.3 2.5 4.1 2.5"/>
                        </svg>
                      </span>
                      <span class="machine-action-copy">
                        <b>Вода</b>
                        <small id="waterActionState">Выключено</small>
                      </span>
                    </button>

                    <button id="steamAction" class="quick-action steam" type="button"
                      onclick="toggleSteamMode()" aria-label="Пар" title="Пар">
                      <span class="quick-icon" aria-hidden="true">
                        <svg viewBox="0 0 32 32">
                          <path d="M9 27c-4-4 4-6.5 0-10.5S13 10 9 6"/>
                          <path d="M16 27c-4-4 4-6.5 0-10.5S20 10 16 6"/>
                          <path d="M23 27c-4-4 4-6.5 0-10.5S27 10 23 6"/>
                        </svg>
                      </span>
                      <span class="machine-action-copy">
                        <b>Пар</b>
                        <small id="steamActionState">Выключено</small>
                      </span>
                    </button>
                  </div>
                </div>
              </div>
            </div>

            <div id="homePressureCard" data-home-info-card="pressure"
              class="card home-pressure-hero pressure-variant-a"
              role="status" aria-label="Давление">
              <div class="home-pressure-gauge">
                <canvas id="homePressureGauge"
                  aria-label="Текущее давление"></canvas>
                <div class="home-pressure-reading pressure-reading-stacked">
                  <strong id="pressure">—</strong>
                  <em>бар</em>
                </div>
              </div>

              <div id="homePressureBackflush" class="home-pressure-backflush" aria-live="polite">
                <span class="home-pressure-backflush-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24">
                    <path d="M12 3.5s-5 5.4-5 9.4a5 5 0 0 0 10 0c0-4-5-9.4-5-9.4Z"/>
                    <path d="M9.7 14.2c.4 1.2 1.2 1.8 2.4 1.9"/>
                  </svg>
                </span>
                <span id="homePressureBackflushText">—</span>
              </div>

              <span id="homePressureTarget" hidden>—</span>
            </div>

            <div id="homeCoffeeStatsCard" data-home-info-card="stats" class="card home-coffee-stats-card"
              role="status" aria-label="Кофе и обслуживание">
              <div class="home-coffee-stat home-coffee-stat-month">
                <span class="home-coffee-stat-label">Кофе за месяц</span>
                <strong id="homeMonthlyShots">—</strong>
                <small><span id="homeMonthlyWeight">—</span> г напитка</small>
              </div>
              <div id="homeBackflushStat" class="home-coffee-stat home-coffee-stat-backflush">
                <span class="home-coffee-stat-label">До промывки</span>
                <strong id="homeBackflushRemaining">—</strong>
                <small id="homeBackflushStatCaption">проливов</small>
              </div>
            </div>

            <div id="temperatureMetricOpen" data-home-info-card="temperature"
              class="card home-metrics-panel home-temperature-hero"
              role="button" tabindex="0" aria-expanded="false"
              aria-controls="temperatureDetailsCard"
              onclick="toggleTemperatureDetails()"
              onkeydown="if(event.key==='Enter'||event.key===' '){event.preventDefault();toggleTemperatureDetails();}"
              title="Показать график температуры">
              <div id="homeTemperatureState" class="home-temperature-hero-state">
                <div class="home-temperature-hero-reading">
                  <strong><i id="homeThermalTemp">—</i><em>°C</em></strong>
                  <span class="home-temperature-target">
                    Цель <b id="homeThermalTarget">—</b> °C
                  </span>
                </div>

                <div class="home-temperature-mini-chart">
                  <canvas id="homeTempMiniChart" aria-label="Мини-график температуры"></canvas>
                </div>

                <div class="home-temperature-hero-footer">
                  <span>Нагрев</span>
                  <span class="home-temperature-heat-track" aria-hidden="true">
                    <i id="homeHeatFill"></i>
                  </span>
                  <span id="homeTempDelta" class="home-temperature-delta">Ожидание данных</span>
                </div>
              </div>
            </div>
          </div>
        </div>

        <div class="shot-board-label">Профиль пролива</div>

        <div id="temperatureDetailsCard" class="card temperature-details-card" hidden>
          <div class="section-head temperature-details-head">
            <div>
              <div class="section-title">Температура и нагрев</div>
              <div id="tempChartContext" class="section-subtitle">Последние 10 минут</div>
            </div>
            <div class="temperature-details-actions">
              <div class="chart-switch temperature-window-switch" aria-label="Период графика температуры">
                <button id="tempWindow3Button" class="small" type="button" onclick="setTemperatureWindow(180)">3 мин</button>
                <button id="tempWindow5Button" class="small" type="button" onclick="setTemperatureWindow(300)">5 мин</button>
                <button id="tempWindow10Button" class="small active" type="button" onclick="setTemperatureWindow(600)">10 мин</button>
              </div>
              <button id="temperatureDetailsCloseButton" class="small temperature-details-close" type="button"
                onclick="setTemperatureDetails(false)" aria-label="Скрыть график температуры" title="Скрыть график температуры">×</button>
            </div>
          </div>

          <canvas id="tempChart"></canvas>

          <div class="legend chart-legend-bottom temperature-details-legend">
            <span><i style="background:#e18b48"></i>Температура</span>
            <span><i style="background:#4bd18b"></i>Цель воды</span>
            <span><i style="background:rgba(255,104,104,.6)"></i>Нагрев PID</span>
          </div>
        </div>

        <div class="grid" style="margin-top:0">
          <div id="shotProfileBoard" class="shot-profile-board">
          <div id="shotChartCard" class="card shot-chart-card" data-shot-panel="chart">
            <div class="section-head shot-chart-head">
              <div class="shot-chart-heading">
                <div class="section-title">График пролива</div>
                <div id="chartContext" class="section-subtitle shot-chart-context">Профиль · —</div>
              </div>
              <div class="chart-head-actions">
                <button id="profilePhaseEditButton" class="small profile-phase-edit-button" type="button"
                  onclick="toggleProfilePhaseEdit()" aria-label="Редактировать профиль"
                  title="Редактировать профиль" aria-controls="profilePhaseEditorHost"
                  aria-expanded="false">✎</button>
                <button id="shotLineSettingsButton" class="small chart-line-settings-button" type="button"
                  onclick="toggleShotLineQuickSettings()" aria-label="Настроить линии графика"
                  title="Настроить линии графика" aria-controls="shotLineQuickSettings" aria-expanded="false">⚙</button>
                <button id="chartExpandButton" class="small chart-expand-button" type="button" onclick="toggleChartExpanded()" aria-label="Увеличить график" title="Увеличить график">⤢</button>
              </div>
            </div>

            <div class="shot-chart-footer shot-profile-controls">
              <label id="stopByWeightChip" class="stop-weight-chip stop-weight-footer" title="Остановка по весу">
                <span class="stop-weight-copy">
                  <small>Стоп по весу</small>
                  <b id="homeStopByWeightState" hidden>Выкл</b>
                </span>
                <span class="stop-weight-switch" aria-hidden="true">
                  <input id="stopByWeight" type="checkbox" onchange="updateSwitch(this,'Silvia Stop Brew By Weight')">
                  <i></i>
                </span>
              </label>
              <button class="target-weight-footer home-value-scrub" type="button"
                data-home-scrub="brewTarget"
                aria-label="Изменить температуру кофе"
                title="Тяните влево или вправо для изменения температуры кофе">
                <small>Температура кофе</small>
                <b id="homeSummaryBrewTarget">—</b>
              </button>
              <button class="target-weight-footer home-value-scrub" type="button"
                data-home-scrub="targetWeight"
                aria-label="Изменить целевой вес"
                title="Тяните влево или вправо для изменения целевого веса">
                <small id="homeSummaryTargetWeightLabel">Целевой вес</small>
                <b id="homeSummaryTargetWeight">—</b>
              </button>
            </div>

            <div class="shot-chart-plot">
                <div id="shotProfileSummary" class="profile-phase-view-strip"
                  aria-label="Этапы выбранного профиля"></div>

                <div id="homeArchiveSummary"
                  class="shot-profile-summary shot-profile-summary-in-chart home-archive-summary"
                  hidden>
                  <div class="shot-profile-chip home-archive-metric">
                    <small>Вес</small><b id="homeArchiveDrinkWeight">—</b>
                  </div>
                  <div class="shot-profile-chip home-archive-metric">
                    <small>Время</small><b id="homeArchiveDuration">—</b>
                  </div>
                  <div class="shot-profile-chip home-archive-metric">
                    <small>Пик давления</small><b id="homeArchivePeakPressure">—</b>
                  </div>
                  <div class="shot-profile-chip home-archive-metric">
                    <small>Цель веса</small><b id="homeArchiveTargetWeight">—</b>
                  </div>
                </div>

                <div class="chart-wrap">
                  <canvas id="chart"></canvas>
                  <div id="chartTooltip" class="chart-tooltip" hidden></div>
                </div>
                <div id="profilePhaseEditorHost"></div>
                <div id="shotLineQuickSettings" class="shot-line-quick-panel" hidden>
                  <div class="shot-line-quick-head">
                    <div>
                      <b>Настройки линий</b>
                      <small>Цвет, толщина и прозрачность</small>
                    </div>
                    <div class="shot-line-quick-actions">
                      <button class="small" type="button" onclick="resetShotLineQuickDraft()" title="Сбросить линии">↺ Сброс</button>
                      <button class="small shot-line-quick-apply" type="button" onclick="applyShotLineQuickSettings()" title="Применить настройки">✓ Применить</button>
                    </div>
                  </div>

                  <div class="shot-line-quick-grid">
                    <div class="shot-line-quick-row">
                      <b>Профиль</b>
                      <label class="shot-line-color-control">
                        <span>Цвет</span>
                        <input id="shotLineQuickProfileColor" type="color" value="#ffd166"
                          oninput="previewShotLineSetting('profile','color',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Толщина <output id="shotLineQuickProfileWidthValue">2.6 px</output></span>
                        <input id="shotLineQuickProfileWidth" type="range" min="0.5" max="5" step="0.1" value="2.6"
                          oninput="previewShotLineSetting('profile','width',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Прозрачность <output id="shotLineQuickProfileTransparencyValue">0%</output></span>
                        <input id="shotLineQuickProfileTransparency" type="range" min="0" max="90" step="5" value="0"
                          oninput="previewShotLineSetting('profile','transparency',this.value)">
                      </label>
                    </div>

                    <div class="shot-line-quick-row">
                      <b>Границы этапов</b>
                      <label class="shot-line-color-control">
                        <span>Цвет</span>
                        <input id="shotLineQuickStagesColor" type="color" value="#8795a8"
                          oninput="previewShotLineSetting('stages','color',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Толщина <output id="shotLineQuickStagesWidthValue">1.2 px</output></span>
                        <input id="shotLineQuickStagesWidth" type="range" min="0.5" max="5" step="0.1" value="1.2"
                          oninput="previewShotLineSetting('stages','width',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Прозрачность <output id="shotLineQuickStagesTransparencyValue">35%</output></span>
                        <input id="shotLineQuickStagesTransparency" type="range" min="0" max="90" step="5" value="35"
                          oninput="previewShotLineSetting('stages','transparency',this.value)">
                      </label>
                    </div>

                    <div class="shot-line-quick-row">
                      <b>Давление</b>
                      <label class="shot-line-color-control">
                        <span>Цвет</span>
                        <input id="shotLineQuickPressureColor" type="color" value="#54a9ff"
                          oninput="previewShotLineSetting('pressure','color',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Толщина <output id="shotLineQuickPressureWidthValue">2.2 px</output></span>
                        <input id="shotLineQuickPressureWidth" type="range" min="0.5" max="5" step="0.1" value="2.2"
                          oninput="previewShotLineSetting('pressure','width',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Прозрачность <output id="shotLineQuickPressureTransparencyValue">0%</output></span>
                        <input id="shotLineQuickPressureTransparency" type="range" min="0" max="90" step="5" value="0"
                          oninput="previewShotLineSetting('pressure','transparency',this.value)">
                      </label>
                    </div>

                    <div class="shot-line-quick-row">
                      <b>Поток</b>
                      <label class="shot-line-color-control">
                        <span>Цвет</span>
                        <input id="shotLineQuickFlowColor" type="color" value="#4bd18b"
                          oninput="previewShotLineSetting('flow','color',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Толщина <output id="shotLineQuickFlowWidthValue">1.8 px</output></span>
                        <input id="shotLineQuickFlowWidth" type="range" min="0.5" max="5" step="0.1" value="1.8"
                          oninput="previewShotLineSetting('flow','width',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Прозрачность <output id="shotLineQuickFlowTransparencyValue">0%</output></span>
                        <input id="shotLineQuickFlowTransparency" type="range" min="0" max="90" step="5" value="0"
                          oninput="previewShotLineSetting('flow','transparency',this.value)">
                      </label>
                    </div>

                    <div class="shot-line-quick-row">
                      <b>Помпа</b>
                      <label class="shot-line-color-control">
                        <span>Цвет</span>
                        <input id="shotLineQuickPumpColor" type="color" value="#9a72ef"
                          oninput="previewShotLineSetting('pump','color',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Толщина <output id="shotLineQuickPumpWidthValue">1.4 px</output></span>
                        <input id="shotLineQuickPumpWidth" type="range" min="0.5" max="5" step="0.1" value="1.4"
                          oninput="previewShotLineSetting('pump','width',this.value)">
                      </label>
                      <label class="shot-line-quick-control">
                        <span>Прозрачность <output id="shotLineQuickPumpTransparencyValue">65%</output></span>
                        <input id="shotLineQuickPumpTransparency" type="range" min="0" max="90" step="5" value="65"
                          oninput="previewShotLineSetting('pump','transparency',this.value)">
                      </label>
                    </div>
                  </div>
                </div>
                <div class="legend chart-legend-bottom shot-legend-split">
                  <div class="legend-group legend-group-left">
                    <button class="shot-legend-toggle" type="button" data-shot-line-visibility="profile" onclick="toggleShotLineVisibility('profile')" aria-pressed="true" title="Скрыть или показать линию профиля"><i data-shot-line-indicator="profile" style="width:18px;height:0;border-radius:0;border-top:3px dashed #ffd166;background:transparent;vertical-align:middle;margin-bottom:2px"></i>Профиль</button>
                    <button class="shot-legend-toggle" type="button" data-shot-line-visibility="pressure" onclick="toggleShotLineVisibility('pressure')" aria-pressed="true" title="Скрыть или показать линию давления"><i data-shot-line-indicator="pressure" style="background:#54a9ff"></i>Давление</button>
                  </div>
                  <div class="legend-group legend-group-right">
                    <button class="shot-legend-toggle" type="button" data-shot-line-visibility="flow" onclick="toggleShotLineVisibility('flow')" aria-pressed="true" title="Скрыть или показать линию потока"><i data-shot-line-indicator="flow" style="background:#4bd18b"></i>Поток, г/с</button>
                    <button class="shot-legend-toggle" type="button" data-shot-line-visibility="pump" onclick="toggleShotLineVisibility('pump')" aria-pressed="true" title="Скрыть или показать линию помпы"><i data-shot-line-indicator="pump" style="background:#9a72ef;opacity:.35"></i>Помпа</button>
                  </div>
                </div>

                <section id="shotQualityCard" class="shot-quality-card" hidden
                  aria-live="polite">
                  <div class="shot-quality-main">
                    <div id="shotQualityScore" class="shot-quality-score">—</div>
                    <div class="shot-quality-copy">
                      <small>Оценка пролива</small>
                      <b id="shotQualityTitle">—</b>
                      <p id="shotQualityDiagnosis">—</p>
                    </div>
                    <button id="shotQualityToggle" class="small shot-quality-toggle"
                      type="button" onclick="toggleShotQualityDetails()"
                      aria-expanded="false" aria-controls="shotQualityDetails">Подробнее</button>
                  </div>
                  <div id="shotQualityDetails" class="shot-quality-details" hidden>
                    <div><span>Рекомендация по помолу</span><b id="shotQualityGrind">—</b></div>
                    <div><span>Уверенность датчика</span><b id="shotQualityConfidence">—</b></div>
                    <div><span>Средняя ошибка давления</span><b id="shotQualityPressureError">—</b></div>
                    <div><span>Средний поток</span><b id="shotQualityFlow">—</b></div>
                  </div>
                </section>
              </div>
          </div>
          <aside id="profileLibraryCard" class="card shot-profile-card profile-library home-shot-rail" data-shot-panel="profiles">
            <div class="home-shot-rail-top">
              <div class="home-shot-rail-tabs" role="tablist" aria-label="Профили и проливы">
                <button id="homeProfilesTab" class="active" type="button" role="tab"
                  aria-selected="true" aria-controls="homeProfilesPane"
                  onclick="setHomeShotRailMode('profiles')">
                  <span id="homeProfilesTabLabel">Профили</span>
                </button>
                <button id="homeShotsTab" type="button" role="tab"
                  aria-selected="false" aria-controls="homeShotsPane"
                  onclick="setHomeShotRailMode('shots')">
                  <span id="homeShotsTabLabel">Проливы</span>
                  <span id="homeArchiveCount" class="home-shot-tab-count">0</span>
                </button>
              </div>
              <div class="home-shot-rail-actions" aria-label="Расположение карточки"></div>
            </div>

            <div id="homeProfilesPane" class="home-shot-rail-pane home-profile-rail-pane" role="tabpanel">
              <label class="home-profile-picker chart-profile-picker profile-rail-select" title="Выбрать профиль">
                <span>Профиль</span>
                <select id="homeProfileSelect" aria-label="Выбрать профиль" onchange="applyHomeProfile(this)"></select>
              </label>

              <div class="profile-library-section built-in-profile-section">
                <div id="homeProfileButtons" class="home-profile-buttons"></div>
              </div>

              <div class="profile-library-divider" aria-hidden="true"></div>

              <div class="profile-library-section user-profile-section">
                <div class="profile-library-title user-profile-title">
                  <b>
                    Мои
                    <span class="profile-count-badge"><span id="userProfileCount">0</span></span>
                  </b>
                  <small id="userProfileHint">Хранятся на кофемашине</small>
                </div>

                <div class="user-profile-save">
                  <input id="userProfileName" type="text" maxlength="36" placeholder="Название профиля"
                    oninput="this.classList.remove('attention')"
                    onkeydown="if(event.key==='Enter'){event.preventDefault();saveCurrentUserProfile();}">
                  <button id="userProfileSaveButton" class="primary" type="button" onclick="saveCurrentUserProfile()"
                    aria-label="Сохранить текущий профиль" title="Сохранить текущий профиль">＋</button>
                </div>

                <div id="userProfileList" class="user-profile-list"></div>
              </div>
            </div>

            <div id="homeShotsPane" class="home-shot-rail-pane home-archive-rail-pane" role="tabpanel" hidden>
              <div class="home-archive-rail-head">
                <div>
                  <b>Сохранённые проливы</b>
                  <small id="homeArchiveStatus">Выберите запись</small>
                </div>
                <div class="home-archive-head-actions">
                  <button type="button" onclick="loadArchiveList(true)"
                    aria-label="Обновить проливы" title="Обновить проливы">
                    <svg viewBox="0 0 24 24" aria-hidden="true">
                      <path d="M20 6v5h-5"/><path d="M4 18v-5h5"/>
                      <path d="M6.1 8.2A7 7 0 0 1 18.8 7L20 11"/>
                      <path d="M17.9 15.8A7 7 0 0 1 5.2 17L4 13"/>
                    </svg>
                  </button>
                </div>
              </div>

              <div id="homeArchiveList" class="history-list home-archive-list">
                <div class="history-list-state">Загрузка истории…</div>
              </div>
            </div>
          </aside>
        </div>
        </div>

      </section>


      <div class="profile-state-storage" hidden aria-hidden="true">
        <select id="profileSelect"></select>
        <input id="brewTarget" type="number" value="0">
        <input id="targetWeight" type="number" value="0">
        <input id="shotSeconds" type="number" value="0">
        <input id="prePump" type="number" value="0">
        <input id="prePause" type="number" value="0">
        <input id="prePower" type="number" value="0">
        <input id="mainPressure" type="number" value="0">
        <input id="endPressure" type="number" value="0">
        <input id="softInfusionTime" type="number" value="0">
        <input id="softInfusionStartPower" type="number" value="0">
      </div>

      <section id="page-settings" class="page">
        <div class="settings-page-head">
          <h1>Настройки</h1>
          <p>Параметры управления машиной</p>
        </div>

        <div class="settings-workspace">
          <aside class="card settings-nav-card">
            <div class="settings-nav-head">
              <b>Категории</b>
              <small>3 раздела</small>
            </div>

            <div class="settings-nav-list" role="tablist" aria-label="Категории настроек">
              <button class="settings-nav-item active" type="button" role="tab"
                data-settings-target="machine" aria-selected="true"
                onclick="setSettingsPanel('machine')">
                <span class="settings-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><rect x="5" y="3" width="14" height="18" rx="2"/><path d="M8 7h8"/><path d="M9 12h6"/><path d="M8 17h8"/></svg>
                </span>
                <span><b>Машина</b><small>Основные настройки</small></span>
                <i>›</i>
              </button>

              <button class="settings-nav-item" type="button" role="tab"
                data-settings-target="temperature" aria-selected="false"
                onclick="setSettingsPanel('temperature')">
                <span class="settings-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M9 14.5V5a3 3 0 0 1 6 0v9.5a5 5 0 1 1-6 0Z"/><path d="M12 7v9"/></svg>
                </span>
                <span><b>Температура</b><small>PID и прогрев</small></span>
                <i>›</i>
              </button>

              <button class="settings-nav-item" type="button" role="tab"
                data-settings-target="pressure" aria-selected="false"
                onclick="setSettingsPanel('pressure')">
                <span class="settings-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M4 16a8 8 0 0 1 16 0"/><path d="M12 16l4-5"/><circle cx="12" cy="16" r="1.4"/><path d="M7 19h10"/></svg>
                </span>
                <span><b>Давление</b><small>Регулятор и наполнение</small></span>
                <i>›</i>
              </button>
            </div>
          </aside>

          <article class="card settings-detail-card">
            <section class="settings-panel" data-settings-panel="temperature" hidden>
              <div class="settings-panel-head">
                <div>
                  <div class="section-title">Температура и PID бойлера</div>
                  <div class="section-subtitle">PID нагревателя, прогрев и компенсация температуры во время пролива.</div>
                </div>
              </div>

              <div class="temperature-settings-grid">
                <section class="temperature-settings-section temperature-settings-section-wide">
                  <div class="temperature-settings-section-head">
                    <b>PID бойлера</b>
                    <small>Три коэффициента регулируют мощность нагревателя по температуре датчика на бойлере.</small>
                  </div>

                  <div class="pid-parameter-grid">
                    <div class="pid-parameter-card">
                      <div class="pid-parameter-main">
                        <label for="pidKp">KP</label>
                        <input id="pidKp" type="number" min="0" max="50" step="0.00001">
                      </div>
                      <small>Сила реакции на текущую ошибку температуры.</small>
                    </div>

                    <div class="pid-parameter-card">
                      <div class="pid-parameter-main">
                        <label for="pidKi">KI</label>
                        <input id="pidKi" type="number" min="0" max="5" step="0.00001">
                      </div>
                      <small>Убирает постоянное небольшое отклонение от цели.</small>
                    </div>

                    <div class="pid-parameter-card">
                      <div class="pid-parameter-main">
                        <label for="pidKd">KD</label>
                        <input id="pidKd" type="number" min="0" max="100" step="0.00001">
                      </div>
                      <small>Сдерживает разгон при быстром приближении к цели.</small>
                    </div>
                  </div>

                  <div class="temperature-autotune-row">
                    <div class="temperature-setting-copy">
                      <b>Автонастройка PID</b>
                      <small>Автоматически подбирает KP, KI и KD. Запускать на включённой машине под наблюдением.</small>
                    </div>
                    <strong id="autotuneStatus">—</strong>
                    <button class="warning" onclick="pressButton('Silvia PID Autotune',tr('Запустить автонастройку PID? Машина должна быть включена и находиться под наблюдением.'))">Запустить</button>
                  </div>
                </section>

                <section class="temperature-settings-section">
                  <div class="temperature-settings-section-head">
                    <b>Прогрев</b>
                    <small>Защищает PID от накопления интеграла, пока бойлер ещё далеко от целевой температуры.</small>
                  </div>

                  <div class="temperature-setting-row">
                    <div class="temperature-setting-copy">
                      <label for="pidWarmupRange">Зона включения интеграла</label>
                      <small>I-часть включается, когда до цели остаётся меньше этого значения. Например: 93 °C и 10 °C → примерно с 83 °C.</small>
                    </div>
                    <div class="temperature-setting-control">
                      <input id="pidWarmupRange" type="number" min="2" max="25" step="0.5">
                      <span>°C</span>
                    </div>
                  </div>

                  <div class="temperature-setting-row temperature-status-row">
                    <div class="temperature-setting-copy">
                      <b>Интеграл PID сейчас</b>
                      <small>Меняется автоматически: на далёком прогреве временно отключён, возле цели работает.</small>
                    </div>
                    <strong id="warmupStatus">—</strong>
                  </div>
                </section>

                <section class="temperature-settings-section">
                  <div class="temperature-settings-section-head">
                    <b>Компенсация при проливе</b>
                    <small>Дополнительный нагрев помогает компенсировать приток холодной воды во время заваривания.</small>
                  </div>

                  <div class="temperature-setting-row">
                    <div class="temperature-setting-copy">
                      <label for="tempFeedForwardMax">Feed-forward максимум</label>
                      <small>Ограничивает дополнительную мощность нагрева во время пролива. Это не PID-коэффициент.</small>
                    </div>
                    <div class="temperature-setting-control">
                      <input id="tempFeedForwardMax" type="number" min="0" max="100" step="5">
                      <span>%</span>
                    </div>
                  </div>
                </section>
              </div>

              <div class="settings-actions temperature-settings-save">
                <button class="primary" onclick="saveNumberGroup(temperatureNums,tr('Настройки температуры сохранены'))">Сохранить настройки температуры</button>
              </div>
            </section>

            <section class="settings-panel" data-settings-panel="pressure" hidden>
              <div class="settings-panel-head">
                <div>
                  <div class="section-title">Регулирование давления</div>
                  <div class="section-subtitle">PI-регулятор управляет мощностью помпы по фактическому давлению и динамике его роста.</div>
                </div>
              </div>

              <div class="pressure-settings-grid">
                <section class="temperature-settings-section pressure-settings-section-wide">
                  <div class="temperature-settings-section-head">
                    <b>PI-регулятор давления</b>
                    <small>Поддерживает давление профиля, изменяя мощность помпы по ошибке давления. Настройки действуют для всех профилей.</small>
                  </div>

                  <div class="pressure-parameter-grid">
                    <div class="pid-parameter-card">
                      <div class="pid-parameter-main">
                        <label for="pressureKp">KP</label>
                        <input id="pressureKp" type="number" min="0" max="0.2" step="0.001">
                      </div>
                      <small>Определяет, насколько сильно помпа реагирует на текущую разницу между целью и фактическим давлением.</small>
                    </div>

                    <div class="pid-parameter-card">
                      <div class="pid-parameter-main">
                        <label for="pressureKi">KI</label>
                        <input id="pressureKi" type="number" min="0" max="0.1" step="0.001">
                      </div>
                      <small>Убирает устойчивое отклонение от цели и помогает регулятору подстроиться под сопротивление кофейной таблетки.</small>
                    </div>
                  </div>
                </section>

                <section class="temperature-settings-section">
                  <div class="temperature-settings-section-head">
                    <b>Мягкое начало пролива</b>
                    <small>Общая плавная оболочка запуска помпы. Это не предсмачивание профиля, а ограничение резкого старта для любого профиля.</small>
                  </div>

                  <div class="temperature-setting-row">
                    <div class="temperature-setting-copy">
                      <label for="pressureSoftInfusionTime">Длительность плавного старта</label>
                      <small>За это время допустимая мощность помпы увеличивается постепенно. Значение 0 с отключает мягкий старт.</small>
                    </div>
                    <div class="temperature-setting-control">
                      <input id="pressureSoftInfusionTime" type="number" min="0" max="5" step="0.5">
                      <span>с</span>
                    </div>
                  </div>

                  <div class="temperature-setting-row">
                    <div class="temperature-setting-copy">
                      <label for="pressureSoftInfusionStartPower">Начальная мощность помпы</label>
                      <small>Стартовая граница мощности, с которой начинается плавный разгон перед обычным регулированием давления.</small>
                    </div>
                    <div class="temperature-setting-control">
                      <input id="pressureSoftInfusionStartPower" type="number" min="5" max="60" step="5">
                      <span>%</span>
                    </div>
                  </div>
                </section>

                <section class="temperature-settings-section pressure-live-section">
                  <div class="temperature-settings-section-head">
                    <b>Состояние регулятора</b>
                    <small>Живые диагностические значения. Они рассчитываются автоматически и здесь не настраиваются.</small>
                  </div>

                  <div class="temperature-setting-row temperature-status-row">
                    <div class="temperature-setting-copy">
                      <b>Адаптивная коррекция помпы</b>
                      <small>Текущая автоматическая добавка к управлению помпой с учётом поведения давления.</small>
                    </div>
                    <strong><i id="pressureFeedForward">—</i> %</strong>
                  </div>

                  <div class="temperature-setting-row temperature-status-row">
                    <div class="temperature-setting-copy">
                      <b>Рост давления</b>
                      <small>Насколько быстро прямо сейчас изменяется давление; регулятор использует динамику, чтобы заранее тормозить разгон.</small>
                    </div>
                    <strong><i id="pressureRiseRate">—</i> бар/с</strong>
                  </div>
                </section>
              </div>

              <div class="settings-actions pressure-settings-save">
                <button class="primary" onclick="saveNumberGroup(pressureNums,tr('Настройки давления сохранены'))">Сохранить настройки давления</button>
              </div>
            </section>

            <section class="settings-panel active" data-settings-panel="machine">
              <div class="settings-panel-head">
                <div>
                  <div class="section-title">Машина</div>
                  <div class="section-subtitle">Основные параметры, которые обычно меняют при эксплуатации.</div>
                </div>
              </div>

              <div class="machine-primary-grid">
                <div class="temperature-setting-row machine-primary-row">
                  <div class="temperature-setting-copy">
                    <label for="brewTempOffset">Поправка воды</label>
                    <small>Датчик стоит на бойлере. Эта поправка связывает его температуру с расчётной температурой воды при заваривании.</small>
                  </div>
                  <div class="temperature-setting-control">
                    <input id="brewTempOffset" type="number" min="-20" max="30" step="0.1">
                    <span>°C</span>
                  </div>
                </div>

                <div class="temperature-setting-row machine-primary-row">
                  <div class="temperature-setting-copy">
                    <label for="steamTarget">Цель пара</label>
                    <small>Целевая температура бойлера после включения режима пара.</small>
                  </div>
                  <div class="temperature-setting-control">
                    <input id="steamTarget" type="number" min="120" max="150" step="1">
                    <span>°C</span>
                  </div>
                </div>

                <div class="temperature-setting-row machine-primary-row">
                  <div class="temperature-setting-copy">
                    <label for="autoOff">Автоотключение</label>
                    <small>Время бездействия до автоматического выключения кофемашины.</small>
                    <span class="machine-live-meta">Осталось: <b id="autoOffRemaining">—</b> мин</span>
                  </div>
                  <div class="temperature-setting-control">
                    <input id="autoOff" type="number" min="0" max="180" step="5">
                    <span>мин</span>
                  </div>
                </div>

                <div class="temperature-setting-row machine-primary-row">
                  <div class="temperature-setting-copy">
                    <label for="dose">Доза кофе</label>
                    <small>Количество сухого кофе на один пролив. Используется для учёта расхода кофе.</small>
                  </div>
                  <div class="temperature-setting-control">
                    <input id="dose" type="number" min="0" max="40" step="0.1">
                    <span>г</span>
                  </div>
                </div>
              </div>

              <div class="machine-primary-actions">
                <button class="secondary" onclick="pressButton('Silvia Reset Auto Off Timer')">Сбросить таймер</button>
                <button class="primary" onclick="saveNumberGroup(machineNums,tr('Настройки машины сохранены'))">Сохранить настройки машины</button>
              </div>
            </section>

                      </article>
        </div>
      </section>

      <section id="page-maintenance" class="page">
        <div class="service-page-head">
          <h1>Обслуживание</h1>
          <p>Весы, промывка, запись пролива и устройство</p>
        </div>

        <div class="service-workspace">
          <aside class="card service-nav-card">
            <div class="service-nav-head">
              <b>Категории</b>
              <small>4 раздела</small>
            </div>

            <div class="service-nav-list" role="tablist" aria-label="Категории обслуживания">
              <button class="service-nav-item active" type="button" role="tab"
                data-maintenance-target="scale" aria-selected="true"
                onclick="setMaintenancePanel('scale')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M7 8h10l1.2 12H5.8L7 8Z"/><path d="M9 8a3 3 0 0 1 6 0"/><path d="M12 11v3"/></svg>
                </span>
                <span><b>Весы</b><small>Тара и калибровка</small></span>
                <i>›</i>
              </button>

              <button class="service-nav-item" type="button" role="tab"
                data-maintenance-target="backflush" aria-selected="false"
                onclick="setMaintenancePanel('backflush')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M20 7v5h-5"/><path d="M4 17v-5h5"/><path d="M6.2 8.3A7 7 0 0 1 18.7 7L20 12"/><path d="M17.8 15.7A7 7 0 0 1 5.3 17L4 12"/></svg>
                </span>
                <span><b>Промывка</b><small>Цикл и напоминание</small></span>
                <i>›</i>
              </button>

              <button class="service-nav-item" type="button" role="tab"
                data-maintenance-target="shotlog" aria-selected="false"
                onclick="setMaintenancePanel('shotlog')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M4 4h16v16H4z"/><path d="M7 15h2l2-6 2 8 2-4h2"/></svg>
                </span>
                <span><b>Запись пролива</b><small>Диагностический CSV</small></span>
                <i>›</i>
              </button>

              <button class="service-nav-item" type="button" role="tab"
                data-maintenance-target="device" aria-selected="false"
                onclick="setMaintenancePanel('device')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><rect x="5" y="3" width="14" height="18" rx="2"/><path d="M8 7h8"/><path d="M9 12h6"/><circle cx="12" cy="17" r="1"/></svg>
                </span>
                <span><b>Устройство</b><small>Счётчики и перезапуск</small></span>
                <i>›</i>
              </button>
            </div>
          </aside>

          <article class="card service-detail-card">
            <section class="service-panel active" data-maintenance-panel="scale">
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Весы</div>
                  <div class="section-subtitle">Тара и калибровка весов.</div>
                </div>
              </div>

              <div class="service-subsection-title">Калибровка</div>
              <div class="service-fields">
                <div class="setting">
                  <div class="service-setting-copy">
                    <label for="scaleCalibrationWeight">Калибровочный вес</label>
                    <small class="service-help-text">Укажите точную массу гири или другого известного груза, который будет использован для калибровки.</small>
                  </div>
                  <div class="service-setting-control">
                    <input id="scaleCalibrationWeight" type="number" min="20" max="2000" step="1">
                    <span>г</span>
                  </div>
                </div>
              </div>
              <div class="service-actions">
                <button class="secondary" onclick="pressButton('Silvia Scale Tare')">Тарировать</button>
                <button class="primary" onclick="calibrateScale()">Калибровать</button>
              </div>
              <div class="service-note service-help-text">
                Сначала оставьте весы пустыми и нажмите «Тарировать», затем положите указанный вес и нажмите «Калибровать».
              </div>

              <div class="service-subsection-title">Автоматика весов</div>
              <div class="service-fields">
                <div class="toggle-setting service-wide">
                  <div class="toggle-copy">
                    <span>Тарировать перед проливом</span>
                    <small class="service-help-text">Перед запуском пролива автоматически обнуляет весы, когда показания стабильны.</small>
                  </div>
                  <label class="toggle-control">
                    <input id="scaleAutoTare" type="checkbox" onchange="updateSwitch(this,'Silvia Scale Auto Tare')">
                    <i></i>
                  </label>
                </div>
              </div>
            </section>

            <section class="service-panel" data-maintenance-panel="backflush" hidden>
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Промывка</div>
                  <div class="section-subtitle">Напоминание и автоматический цикл обратной промывки.</div>
                </div>
              </div>

              <div class="service-subsection-title">Параметры цикла</div>
              <div class="service-fields">
                <div class="setting">
                  <div class="service-setting-copy">
                    <label for="backflushReminder">Напоминание</label>
                    <small class="service-help-text">Через сколько проливов после последней промывки показать напоминание. Значение 0 отключает напоминание.</small>
                  </div>
                  <div class="service-setting-control">
                    <input id="backflushReminder" type="number" min="0" max="150" step="1">
                    <span>проливов</span>
                  </div>
                </div>
                <div class="setting">
                  <div class="service-setting-copy">
                    <label for="backflushRinseDelay">Пауза перед ополаскиванием</label>
                    <small class="service-help-text">Задержка между основной частью автоматической промывки и этапом ополаскивания.</small>
                  </div>
                  <div class="service-setting-control">
                    <input id="backflushRinseDelay" type="number" min="30" max="180" step="5">
                    <span>с</span>
                  </div>
                </div>
              </div>
              <div class="service-actions">
                <button class="primary" onclick="saveNumberGroup(backflushNums,tr('Параметры обслуживания сохранены'))">Сохранить параметры</button>
              </div>

              <div class="service-subsection-title">Состояние</div>
              <div class="service-status-grid">
                <div><span>Статус</span><b id="backflushStatus">—</b></div>
                <div><span>После промывки</span><b id="backflushShots">—</b></div>
                <div><span>Всего проливов</span><b id="lifetimeShots">—</b></div>
              </div>

              <div class="service-subsection-title">Управление циклом</div>
              <p class="service-group-help service-help-text">Для обратной промывки установите глухую корзину. Запуск и остановка выполняются контроллером автоматически по сервисному циклу.</p>
              <div class="service-actions service-actions-danger">
                <button class="warning" onclick="pressButton('Silvia Start Backflush',tr('Запустить автоматическую обратную промывку? Установи глухую корзину и не оставляй машину без присмотра.'))">Запустить</button>
                <button class="danger" onclick="pressButton('Silvia Stop Backflush')">Остановить</button>
                <button class="secondary" onclick="pressButton('Silvia Reset Backflush Shots',tr('Сбросить счётчик проливов после промывки?'))">Сбросить счётчик</button>
              </div>
            </section>

            <section class="service-panel" data-maintenance-panel="shotlog" hidden>
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Запись пролива</div>
                  <div class="section-subtitle">Диагностическая запись внутренних данных регулятора в CSV.</div>
                </div>
              </div>

              <div class="service-subsection-title">Следующий пролив</div>
              <div class="service-fields">
                <div class="toggle-setting service-wide">
                  <div class="toggle-copy">
                    <span>Записать следующий пролив</span>
                    <small class="service-help-text">Включает подробную запись только для следующего пролива, затем переключатель автоматически выключается. Этот CSV предназначен для диагностики и не заменяет обычный архив проливов.</small>
                  </div>
                  <label class="toggle-control">
                    <input id="shotDiagnostics" type="checkbox" onchange="updateSwitch(this,'Silvia Shot Diagnostics')">
                    <i></i>
                  </label>
                </div>
              </div>

              <div class="service-subsection-title">Файл CSV</div>
              <div class="service-status-grid service-status-single">
                <div><span>CSV готов</span><b id="csvReady">—</b></div>
              </div>
              <p class="service-group-help service-help-text">CSV нужен для технического анализа работы регулятора и датчиков во время пролива.</p>
              <div class="service-actions">
                <a class="button primary" href="/shot.csv">Скачать CSV</a>
                <button class="secondary" onclick="pressButton('Silvia Clear Shot Diagnostic CSV',tr('Удалить диагностический CSV из памяти?'))">Очистить</button>
              </div>
            </section>

            <section class="service-panel" data-maintenance-panel="device" hidden>
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Устройство</div>
                  <div class="section-subtitle">Состояние контроллера и сервисные действия.</div>
                </div>
              </div>

              <div class="service-subsection-title">Состояние</div>
              <div class="service-status-grid">
                <div><span>Причина перезапуска</span><b id="resetReason">—</b></div>
                <div><span>Израсходовано кофе</span><b><i id="groundsUsed">—</i> г</b></div>
              </div>
              <p class="service-group-help service-help-text">Причина перезапуска помогает понять, был ли это обычный запуск, программный restart или сбой питания/контроллера.</p>

              <div class="service-subsection-title">Сервисные действия</div>
              <p class="service-group-help service-help-text">Перезапуск ESP32 временно отключит веб-интерфейс и управление машиной, пока контроллер снова не загрузится.</p>
              <div class="service-actions service-actions-danger">
                <button class="secondary" onclick="pressButton('Silvia Reset Auto Off Timer')">Сбросить таймер</button>
                <button class="danger" onclick="pressButton('Silvia ESP Restart',tr('Перезапустить ESP32? Управление машиной временно отключится.'))">Перезапустить ESP32</button>
              </div>
            </section>
          </article>
        </div>
      </section>

      <section id="page-lab" class="page">
        <div class="page-heading">
          <h1>Лаборатория</h1>
          <p>Сбор и строгая проверка данных для будущей модели давления</p>
        </div>

        <div class="lab-grid">
          <article class="card lab-overview">
            <div class="lab-collection-row">
              <div class="lab-collection-copy">
                <b>Сбор данных для модели</b>
                <small>Сохраняет полный CSV каждого пролива и проверяет надёжность XDB401.</small>
              </div>
              <label class="toggle-control">
                <input id="trainingCollectionToggle" type="checkbox"
                  onchange="toggleTrainingCollection(this)">
                <i></i>
              </label>
            </div>

            <section id="trainingReadiness" class="lab-readiness">
              <div class="lab-readiness-head">
                <div>
                  <small>Готовность набора</small>
                  <b id="trainingReadinessTitle">Загрузка…</b>
                </div>
                <span id="trainingReadinessScore" class="lab-readiness-score">—</span>
              </div>
              <div class="lab-progress" aria-hidden="true"><i id="trainingReadinessBar"></i></div>
              <p id="trainingReadinessCopy">Получаем оценки сохранённых проливов.</p>
            </section>

            <div class="lab-metrics">
              <div class="lab-metric"><span>Чистые проливы</span><b id="trainingCleanCount">—</b></div>
              <div class="lab-metric"><span>Пригодные окна</span><b id="trainingWindowCount">—</b></div>
              <div class="lab-metric"><span>Частичные</span><b id="trainingPartialCount">—</b></div>
              <div class="lab-metric"><span>Отклонены</span><b id="trainingRejectedCount">—</b></div>
              <div class="lab-metric"><span>Профили</span><b id="trainingProfileCount">—</b></div>
              <div class="lab-metric"><span>Ошибки XDB401</span><b id="trainingSensorRejectedCount">—</b></div>
            </div>

            <div class="lab-storage">
              <div class="lab-storage-top">
                <span class="lab-storage-label">Хранилище LittleFS</span>
                <b id="trainingStorageText">—</b>
              </div>
              <div class="lab-storage-bar" aria-hidden="true"><i id="trainingStorageBar"></i></div>
              <div id="trainingStorageEstimate" class="lab-export-progress"></div>
            </div>

            <div class="lab-actions">
              <button id="trainingExportButton" class="primary" type="button"
                onclick="exportTrainingDataset(false)">Выгрузить пригодные</button>
              <button id="trainingExportAllButton" class="secondary" type="button"
                onclick="exportTrainingDataset(true)">Выгрузить все собранные</button>
            </div>
            <div id="trainingExportProgress" class="lab-export-progress" aria-live="polite"></div>
          </article>

          <article class="card lab-records">
            <div class="lab-records-head">
              <div class="lab-record-head">
                <b>Проверенные записи</b>
                <small>Звезда защищает запись от автоматического удаления.</small>
              </div>
              <button type="button" onclick="loadTrainingDataset(true)"
                aria-label="Обновить набор" title="Обновить набор">↻</button>
            </div>
            <div id="trainingRecordList" class="lab-record-list">
              <div class="lab-empty">Загрузка записей…</div>
            </div>
          </article>
        </div>
      </section>

      <section id="page-diagnostics" class="page">
        <div class="service-page-head">
          <h1>Диагностика</h1>
          <p>Датчики температуры, давления и веса, регуляторы и ресурсы ESP32</p>
        </div>

        <div class="service-workspace diagnostics-workspace">
          <aside class="card service-nav-card">
            <div class="service-nav-head">
              <b>Категории</b>
              <small>4 раздела</small>
            </div>

            <div class="service-nav-list" role="tablist" aria-label="Категории диагностики">
              <button class="service-nav-item active" type="button" role="tab"
                data-diagnostics-target="pressure" aria-selected="true"
                onclick="setDiagnosticsPanel('pressure')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M4 16a8 8 0 0 1 16 0"/><path d="M12 16l4-5"/><circle cx="12" cy="16" r="1.4"/><path d="M7 19h10"/></svg>
                </span>
                <span><b>XDB401</b><small>Связь, данные и защита</small></span>
                <i>›</i>
              </button>

              <button class="service-nav-item" type="button" role="tab"
                data-diagnostics-target="temperature" aria-selected="false"
                onclick="setDiagnosticsPanel('temperature')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M9 14.5V5a3 3 0 0 1 6 0v9.5a5 5 0 1 1-6 0Z"/><path d="M12 7v9"/></svg>
                </span>
                <span><b>Температура</b><small>PT100 и нагрев</small></span>
                <i>›</i>
              </button>

              <button class="service-nav-item" type="button" role="tab"
                data-diagnostics-target="scale" aria-selected="false"
                onclick="setDiagnosticsPanel('scale')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M7 8h10l1.2 12H5.8L7 8Z"/><path d="M9 8a3 3 0 0 1 6 0"/><path d="M12 11v3"/></svg>
                </span>
                <span><b>Весы</b><small>HX711 и калибровка</small></span>
                <i>›</i>
              </button>

              <button class="service-nav-item" type="button" role="tab"
                data-diagnostics-target="system" aria-selected="false"
                onclick="setDiagnosticsPanel('system')">
                <span class="service-nav-icon" aria-hidden="true">
                  <svg viewBox="0 0 24 24"><path d="M4 5h16v14H4z"/><path d="M7 9h3"/><path d="M7 13h5"/><path d="M7 17h8"/><circle cx="17" cy="9" r="1"/></svg>
                </span>
                <span><b>Система</b><small>ESP32 и память</small></span>
                <i>›</i>
              </button>
            </div>
          </aside>

          <article class="card service-detail-card diagnostics-detail-card">
            <section class="service-panel active" data-diagnostics-panel="pressure">
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Датчик давления XDB401</div>
                  <div class="section-subtitle">Разбор ошибок обмена, качества данных и защитных остановок.</div>
                </div>
              </div>

              <section class="diagnostics-system-section diagnostics-sensor-section diagnostics-sensor-clean">
                <div class="diagnostics-section-label diagnostics-primary-label">Последний пролив</div>
                <div id="xdbLastShotCard"
                  class="xdb-shot-card xdb-shot-card-primary loading">
                  <div class="xdb-shot-head">
                    <div>
                      <b>Последний диагностический CSV</b>
                      <span id="xdbShotState">Проверяем запись последнего пролива…</span>
                    </div>
                    <button class="small secondary" type="button"
                      onclick="analyzeLatestShotCsv(true)">Обновить анализ</button>
                  </div>

                  <p id="xdbShotSummary">
                    После анализа здесь появится вывод о связи XDB401 и работе защиты помпы.
                  </p>

                  <div id="xdbShotFacts" class="xdb-shot-facts" hidden>
                    <div><span>Защита помпы</span><b id="xdbShotProtection">—</b></div>
                    <div><span id="xdbShotEventLabel">Событие</span><b id="xdbShotEventValue">—</b></div>
                    <div><span>Макс. возраст данных</span><b id="xdbShotSensorAge">—</b></div>
                    <div><span>Макс. ошибок подряд</span><b id="xdbShotConsecutive">—</b></div>
                    <div><span>Восстановление связи</span><b id="xdbShotRecovery">—</b></div>
                    <div class="xdb-shot-fact-wide"><span>Источник ошибки</span><b id="xdbShotReason">—</b></div>
                  </div>
                </div>

                <div class="service-actions diagnostics-system-actions diagnostics-csv-actions">
                  <a class="button" href="/shot.csv">Скачать последний CSV</a>
                </div>

                <div class="diagnostics-section-label">Состояние сейчас</div>
                <div id="xdbHumanCard"
                  class="xdb-human-card xdb-human-status checking" role="status"
                  aria-live="polite">
                  <div class="xdb-human-copy">
                    <div class="xdb-human-status-top">
                      <div class="xdb-human-status-title">
                        <span>Связь с датчиком</span>
                        <b id="xdbHumanTitle">Проверяем связь XDB401…</b>
                      </div>
                    </div>
                    <p id="xdbHumanText">Получаем состояние датчика и счётчики обмена.</p>
                    <div class="xdb-human-chips">
                      <span>Возраст данных: <b id="pressureDataAge">—</b> с</span>
                      <span>Ошибок подряд: <b id="xdbHumanConsecutive">—</b></span>
                      <span>Температура: <b id="pressureSensorTemp">—</b> °C</span>
                      <span>Последняя: <b id="xdbHumanLastError">—</b></span>
                    </div>
                  </div>
                </div>

                <div class="diagnostics-section-label">Сводка после запуска ESP32</div>
                <div id="xdbSystemSummary"
                  class="xdb-system-summary xdb-system-summary-compact checking">
                  <div>
                    <b id="xdbSystemSummaryTitle">Накопительная статистика XDB401</b>
                    <p id="xdbSystemSummaryText">
                      Загружаем общие счётчики ошибок с момента запуска контроллера.
                    </p>
                  </div>
                  <span id="xdbSystemDominant">—</span>
                </div>

                <div class="diagnostics-section-label">Счётчики ошибок XDB401</div>
                <div class="diagnostics-metrics diagnostics-system-grid diagnostics-sensor-grid">
                  <div><span>Всего ошибок</span><b id="errTotal">—</b></div>
                  <div><span>Старт измерения</span><b id="errStart">—</b></div>
                  <div><span>Чтение статуса</span><b id="errStatus">—</b></div>
                  <div><span>Тайм-аут измерения</span><b id="errTimeout">—</b></div>
                  <div><span>Чтение пакета</span><b id="errPacket">—</b></div>
                  <div><span>Отбраковано пакетов</span><b id="errRejected">—</b></div>
                  <div><span>Ошибок подряд</span><b id="errConsecutive">—</b></div>
                  <div class="diagnostics-wide"><span>Последняя ошибка XDB401</span><b id="errLast">—</b></div>
                </div>
              </section>
            </section>

            <section class="service-panel" data-diagnostics-panel="temperature" hidden>
              <div class="service-panel-head">
                <div>
                  <div class="section-title">PT100 и управление нагревом</div>
                  <div class="section-subtitle">Сначала проверка датчика температуры, ниже — техническая телеметрия регулятора нагрева.</div>
                </div>
              </div>

              <div class="diagnostics-section-label">Датчик PT100 / MAX31865</div>
              <div class="diagnostics-metrics diagnostics-system-grid">
                <div><span>Температура бойлера PT100</span><b><i id="pt100Temperature">—</i> °C</b></div>
                <div><span>Состояние данных</span><b id="pt100Status">—</b></div>
                <div><span>Возраст последнего значения</span><b><i id="pt100DataAge">—</i> с</b></div>
                <div><span>Сбоев после запуска ESP32</span><b id="pt100FaultCount">—</b></div>
              </div>

              <div class="diagnostics-section-label">Регулятор нагрева</div>
              <p class="service-group-help">Эти значения нужны при настройке PID: P, I и D — его составляющие, «PID до компенсации» — их сумма, а итоговая команда нагревателю может быть выше из-за компенсации притока холодной воды.</p>
              <div class="diagnostics-metrics">
                <div><span>Расчётная температура воды</span><b><i id="diagTemp">—</i> °C</b></div>
                <div><span>Итоговая команда нагревателю</span><b><i id="diagHeat">—</i> %</b></div>
                <div><span>PID до компенсации</span><b id="piOutput">—</b></div>
                <div><span>P — текущая ошибка</span><b id="pidP">—</b></div>
                <div><span>I — накопленная ошибка</span><b id="pidI">—</b></div>
                <div><span>D — скорость изменения</span><b id="pidD">—</b></div>
                <div><span>Компенсация холодной воды</span><b><i id="tempFeedForward">—</i> %</b></div>
                <div><span>Интеграл на прогреве</span><b id="diagWarmup">—</b></div>
                <div><span>Автонастройка PID</span><b id="diagAutotune">—</b></div>
              </div>
            </section>

            <section class="service-panel" data-diagnostics-panel="scale" hidden>
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Весы HX711</div>
                  <div class="section-subtitle">Проверка поступления сырых данных, состояния тары и калибровки.</div>
                </div>
              </div>

              <div class="diagnostics-metrics diagnostics-system-grid">
                <div><span>Состояние данных</span><b id="scaleDiagStatus">—</b></div>
                <div><span>Возраст последнего отсчёта</span><b><i id="scaleDataAge">—</i> с</b></div>
                <div><span>Сбоев после запуска ESP32</span><b id="scaleFaultCount">—</b></div>
                <div><span>Сырой отсчёт HX711</span><b id="scaleRawDiagnostic">—</b></div>
                <div><span>Текущий вес</span><b><i id="scaleDiagnosticWeight">—</i> г</b></div>
              </div>
              <p class="service-group-help">Ноль граммов сам по себе не является ошибкой. Отвал определяется по отсутствию свежих отсчётов или некорректному значению HX711.</p>
            </section>

            <section class="service-panel" data-diagnostics-panel="system" hidden>
              <div class="service-panel-head">
                <div>
                  <div class="section-title">Система</div>
                  <div class="section-subtitle">Память и сервисные данные ESP32.</div>
                </div>
              </div>

              <section class="diagnostics-system-section diagnostics-esp-section">
                <div class="diagnostics-system-section-head">
                  <span class="diagnostics-system-section-icon esp" aria-hidden="true">
                    <svg viewBox="0 0 24 24">
                      <rect x="5" y="5" width="14" height="14" rx="2"/>
                      <path d="M9 9h6v6H9z"/>
                      <path d="M9 2v3M15 2v3M9 19v3M15 19v3"/>
                      <path d="M2 9h3M2 15h3M19 9h3M19 15h3"/>
                    </svg>
                  </span>
                  <div>
                    <b>ESP32 и память</b>
                    <p>Ресурсы контроллера отдельно от ошибок датчика.</p>
                  </div>
                </div>

                <div class="diagnostics-metrics diagnostics-system-grid diagnostics-esp-grid">
                  <div>
                    <span>Свободно PSRAM</span>
                    <b><i id="freePsram">—</i> kB</b>
                  </div>
                  <div>
                    <span>Крупнейший блок PSRAM</span>
                    <b><i id="largestPsram">—</i> kB</b>
                  </div>
                  <div>
                    <span>Свободный внутренний heap</span>
                    <b><i id="freeInternal">—</i> kB</b>
                  </div>
                </div>

                <p class="diagnostics-esp-note">
                  Показатели памяти относятся к ESP32 и не являются ошибками XDB401.
                </p>
              </section>

              <div class="service-actions diagnostics-system-actions">
                <a class="button secondary" href="/?native=1">Стандартная страница ESPHome</a>
              </div>
            </section>
          </article>
        </div>
      </section>
    </main>
  </div>

  <div id="toast" class="toast"></div>


  <style>
    /* Stage 90: horizontal drag editing for shot summary values */
    .home-value-scrub {
      position:relative;
      cursor:ew-resize;
      touch-action:pan-y;
      user-select:none;
      -webkit-user-select:none;
    }
    .home-value-scrub small::after {
      content:" ↔";
      color:#74808d;
      font-size:.9em;
      font-weight:600;
    }
    .home-value-scrub.scrubbing {
      z-index:3;
      border-color:#d08a4f !important;
      background:linear-gradient(145deg,#2b2119,#17191d) !important;
      box-shadow:0 0 0 2px rgba(225,139,72,.12);
    }
    .home-value-scrub.saving {
      cursor:progress;
      opacity:.78;
      border-color:#586675 !important;
    }
    body.home-value-scrubbing {
      cursor:ew-resize;
      user-select:none;
      -webkit-user-select:none;
    }
    @media (hover:hover) {
      .home-value-scrub:hover { border-color:#667385; }
    }

  

    /* Stage 164/165: mascot replaces water drop; low-water threshold semantics */
    #page-home .machine-restored-status .machine-water-indicator.mascot-water-slot {
      position:relative !important;
      flex:0 0 auto !important;
      width:84px !important;
      min-width:84px !important;
      height:58px !important;
      min-height:58px !important;
      display:grid !important;
      place-items:center !important;
      padding:0 !important;
      border:0 !important;
      border-radius:0 !important;
      background:transparent !important;
      box-shadow:none !important;
      overflow:visible !important;
    }

    #page-home .machine-restored-status .mascot-water-slot.ok,
    #page-home .machine-restored-status .mascot-water-slot.warn,
    #page-home .machine-restored-status .mascot-water-slot.unknown {
      border:0 !important;
      background:transparent !important;
      box-shadow:none !important;
    }

    #page-home .mascot-water-slot .machine-mascot {
      width:76px;
      height:52px;
      padding:8px 10px 13px;
      border-radius:15px;
      column-gap:9px;
    }

    #page-home .mascot-water-slot .machine-mascot-eye {
      width:17px;
      height:13px;
      border-width:1.6px;
    }

    #page-home .mascot-water-slot .machine-mascot-pupil {
      width:4.5px;
      height:4.5px;
    }

    #page-home .mascot-water-slot .machine-mascot-mouth {
      bottom:7px;
    }

    #page-home .machine-mascot-water-alert {
      position:absolute;
      z-index:4;
      top:-1px;
      right:0;
      width:20px;
      height:20px;
      display:grid;
      place-items:center;
      border:1px solid #4c5661;
      border-radius:999px;
      background:#212830;
      color:#9aa5b1;
      box-shadow:0 2px 7px rgba(0,0,0,.28);
      font-size:12px;
      line-height:1;
      font-weight:800;
      opacity:0;
      transform:scale(.72);
      pointer-events:none;
      transition:opacity .2s ease, transform .2s ease, border-color .2s ease, background .2s ease, color .2s ease;
    }

    #page-home .mascot-water-slot.unknown .machine-mascot-water-alert {
      opacity:.92;
      transform:scale(1);
    }

    #page-home .mascot-water-slot.warn .machine-mascot-water-alert {
      opacity:1;
      transform:scale(1);
      border-color:#8c454c;
      background:#4a252a;
      color:#ff9398;
      animation:machineMascotWaterAlert 1.65s ease-in-out infinite;
    }

    #page-home .mascot-water-slot.ok .machine-mascot-water-alert {
      opacity:0;
      transform:scale(.72);
      animation:none;
    }

    #page-home .mascot-water-slot.warn .machine-mascot {
      box-shadow:
        inset 0 1px 0 rgba(255,255,255,.04),
        0 0 16px rgba(235,106,106,.09);
    }

    @keyframes machineMascotWaterAlert {
      0%,100% { transform:scale(1); box-shadow:0 2px 7px rgba(0,0,0,.28),0 0 0 0 rgba(255,119,119,.18); }
      50% { transform:scale(1.08); box-shadow:0 2px 7px rgba(0,0,0,.28),0 0 0 5px rgba(255,119,119,0); }
    }

    @media (max-width:760px) {
      #page-home .machine-restored-status .machine-water-indicator.mascot-water-slot {
        width:74px !important;
        min-width:74px !important;
        height:52px !important;
        min-height:52px !important;
      }

      #page-home .mascot-water-slot .machine-mascot {
        width:68px;
        height:48px;
        padding:7px 8px 12px;
        column-gap:7px;
      }

      #page-home .mascot-water-slot .machine-mascot-eye {
        width:15px;
        height:11px;
      }

      #page-home .machine-mascot-water-alert {
        width:18px;
        height:18px;
        top:-1px;
        right:-1px;
        font-size:11px;
      }
    }

    @media (prefers-reduced-motion:reduce) {
      #page-home .machine-mascot-water-alert {
        animation:none !important;
        transition:none !important;
      }
    }

    /* Local post-shot analysis: compact summary first, evidence on demand. */
    #shotQualityCard {
      margin-top:8px;
      overflow:hidden;
      border:1px solid #34404b;
      border-left:3px solid #71808e;
      border-radius:11px;
      background:#11171c;
    }
    #shotQualityCard[hidden],#shotQualityDetails[hidden]{display:none!important}
    #shotQualityCard.good{border-left-color:#4bb77f}
    #shotQualityCard.acceptable{border-left-color:#d4a94d}
    #shotQualityCard.poor,#shotQualityCard.sensor_fault{border-left-color:#df6558}
    .shot-quality-main{min-width:0;display:grid;grid-template-columns:56px minmax(0,1fr) auto;align-items:center;gap:10px;padding:10px 11px}
    .shot-quality-score{width:50px;height:50px;display:grid;place-items:center;border:1px solid #44515d;border-radius:50%;background:#171e24;color:#dce5ec;font-size:var(--zone-font-value,22px);font-weight:800;font-variant-numeric:tabular-nums}
    #shotQualityCard.good .shot-quality-score{border-color:#3d7558;color:#8ae1ad}
    #shotQualityCard.acceptable .shot-quality-score{border-color:#725f31;color:#edc96f}
    #shotQualityCard.poor .shot-quality-score,#shotQualityCard.sensor_fault .shot-quality-score{border-color:#754147;color:#f08a83}
    .shot-quality-copy{min-width:0}
    .shot-quality-copy small{display:block;color:#7f8c98;font-size:var(--zone-font-small,12px);font-weight:700;letter-spacing:.04em;text-transform:uppercase}
    .shot-quality-copy b{display:block;margin-top:2px;color:#dce4eb;font-size:var(--zone-font-text,14px);line-height:1.3}
    .shot-quality-copy p{margin:4px 0 0;overflow:hidden;color:#8f9ba6;font-size:var(--zone-font-small,12px);line-height:1.4;text-overflow:ellipsis;white-space:nowrap}
    .shot-quality-toggle{min-height:34px;padding:7px 10px;border:1px solid #3b4651;background:#1d242b;color:#b8c2cc;font-size:var(--zone-font-small,12px)}
    .shot-quality-details{display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:1px;border-top:1px solid #2b353e;background:#2b353e}
    .shot-quality-details>div{min-width:0;padding:10px 11px;background:#11171c}
    .shot-quality-details span,.shot-quality-details b{display:block;overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
    .shot-quality-details span{color:#788591;font-size:var(--zone-font-small,12px);line-height:1.3}
    .shot-quality-details b{margin-top:4px;color:#cfd8e0;font-size:var(--zone-font-text,14px);font-weight:650;line-height:1.3}
    .archive-row-open{position:relative;padding-right:46px}
    .archive-row-quality{position:absolute;top:9px;right:9px;min-width:32px;height:24px;display:grid;place-items:center;padding:0 6px;border:1px solid #45515d;border-radius:999px;background:#202831;color:#b8c3ce;font-size:var(--zone-font-small,12px);font-weight:800;font-variant-numeric:tabular-nums}
    .archive-row-quality.good{border-color:#386a50;color:#7bdca2}
    .archive-row-quality.acceptable{border-color:#6a592f;color:#e7c46c}
    .archive-row-quality.poor,.archive-row-quality.sensor_fault{border-color:#6d3e44;color:#ee8580}
    @media (max-width:620px){.shot-quality-details{grid-template-columns:repeat(2,minmax(0,1fr))}}
    @media (max-width:430px){.shot-quality-main{grid-template-columns:52px minmax(0,1fr)}.shot-quality-score{width:46px;height:46px;font-size:var(--zone-font-value,22px)}.shot-quality-toggle{grid-column:1/-1;width:100%}.shot-quality-copy p{white-space:normal}.shot-quality-details{grid-template-columns:minmax(0,1fr)}}


    /* Stage 170: reuse the auto-off line for the live shot phase timer. */
    #page-home .machine-status-copy > #shotStatus.run,
    #page-home .machine-restored-status .machine-status-copy > #shotStatus.run {
      display:block !important;
      max-width:100%;
      overflow:hidden !important;
      text-overflow:ellipsis !important;
      white-space:nowrap !important;
    }

    #page-home #shotStatus.run .shot-phase-label {
      display:block;
      max-width:100%;
      overflow:hidden;
      text-overflow:ellipsis;
      white-space:nowrap;
      font-size:inherit;
      line-height:1.05;
    }

    #page-home .machine-auto-off.shot-phase-countdown b {
      font-variant-numeric:tabular-nums;
    }

    /* Keep the pressure value in the visual center of the dial. */
    #page-home .pressure-variant-a .pressure-reading-stacked {
      top:56% !important;
    }

    /* Recipe controls belong with the profile, above its timeline. */
    .shot-profile-controls {
      display:grid;
      grid-template-columns:minmax(132px,.9fr) repeat(2,minmax(120px,1fr));
      align-items:stretch;
      gap:7px;
      min-height:0;
      margin:8px 0 7px;
      padding:6px;
    }
    .shot-profile-controls .stop-weight-footer,
    .shot-profile-controls .target-weight-footer {
      width:100%;
      min-width:0;
      grid-column:auto;
      grid-row:auto;
      margin:0;
    }
    .shot-profile-controls .target-weight-footer {
      justify-content:center;
      padding:5px 6px;
    }
    @media (max-width:600px) {
      .shot-profile-controls {
        grid-template-columns:repeat(3,minmax(0,1fr));
      }
      .shot-profile-controls .target-weight-footer {
        grid-column:auto;
        grid-row:auto;
        justify-content:center;
      }
    }

    #page-home .home-top-grid{grid-template-columns:var(--home-top-grid-columns,minmax(360px,var(--home-left-column-share,38fr)) minmax(145px,var(--home-pressure-column-share,18fr)) minmax(145px,var(--home-stats-column-share,18fr)) minmax(210px,var(--home-right-column-share,26fr)))!important;gap:12px;align-items:stretch}
    #page-home [data-home-info-card]{position:relative;transition:opacity .15s ease,outline-color .15s ease,transform .15s ease}
    #page-home .home-info-order-handle{display:none;position:absolute;top:-11px;left:50%;z-index:16;min-width:42px;height:25px;padding:0 10px;transform:translateX(-50%);border:1px solid #596574;border-radius:999px;background:#262d36;box-shadow:0 5px 16px rgba(0,0,0,.35);color:#d8dee7;font-size:17px;line-height:1;cursor:grab;touch-action:none;user-select:none}
    #page-home.home-layout-editing [data-home-info-card]{margin-top:8px!important;outline:1px dashed #596574;outline-offset:3px}
    #page-home.home-layout-editing .home-info-order-handle{display:inline-grid;place-items:center;pointer-events:auto!important}
    #page-home .home-info-order-handle:active{cursor:grabbing;transform:translateX(-50%) scale(.96)}
    #page-home [data-home-info-card].home-info-card-dragging{opacity:.38;transform:scale(.98)}
    #page-home [data-home-info-card].home-info-card-drop-target{outline-color:#e18b48}
    body.home-info-card-layout-dragging{cursor:grabbing;user-select:none}
    #page-home .home-coffee-stats-card{min-width:0;height:var(--home-temperature-card-height)!important;min-height:var(--home-temperature-card-height)!important;display:grid;grid-template-rows:1fr 1fr;padding:0;overflow:hidden;box-sizing:border-box}
    #page-home .home-coffee-stat{min-width:0;display:flex;flex-direction:column;align-items:center;justify-content:center;padding:12px 9px;text-align:center}
    #page-home .home-coffee-stat+.home-coffee-stat{border-top:1px solid rgba(118,145,167,.18)}
    #page-home .home-coffee-stat-label{color:#8f9aa6;font-size:10px;font-weight:700;line-height:1.15}
    #page-home .home-coffee-stat strong{margin-top:3px;color:#eef2f5;font-size:30px;font-weight:800;line-height:1;font-variant-numeric:tabular-nums}
    #page-home .home-coffee-stat small{min-width:0;margin-top:4px;overflow:hidden;color:#75818d;font-size:9px;line-height:1.15;text-overflow:ellipsis;white-space:nowrap}
    #page-home .home-coffee-stat-month strong{color:#e8bd78}
    #page-home .home-coffee-stat-backflush strong{color:#83c9f4}
    #page-home .home-coffee-stat-backflush.due{background:linear-gradient(145deg,rgba(111,48,38,.42),rgba(63,33,29,.3))}
    #page-home .home-coffee-stat-backflush.due strong,#page-home .home-coffee-stat-backflush.due .home-coffee-stat-label{color:#efaa78}
    #page-home .pressure-variant-a .home-pressure-backflush{display:none}
    @media(max-width:900px){#page-home .home-top-grid{grid-template-columns:repeat(2,minmax(0,1fr))!important}#page-home .machine-panel,#page-home .home-temperature-hero{grid-column:1/-1}#page-home .home-pressure-hero,#page-home .home-coffee-stats-card{min-height:180px}}
    @media(max-width:430px){#page-home .home-pressure-hero,#page-home .home-coffee-stats-card{min-height:165px}#page-home .home-coffee-stat{padding:9px 6px}#page-home .home-coffee-stat strong{font-size:27px}#page-home .home-coffee-stat-label{font-size:9px}}
</style>

  <script>
    if (location.pathname === '/coffee' || location.pathname === '/coffee/') {
      history.replaceState(null, '', '/' + location.search + location.hash);
    }

    const E = {
      'silvia brew pressure': ['pressure', 'num'],
      'silvia target brew pressure': ['target', 'num'],
      'silvia estimated brew temperature': ['temp', 'num'],
      'silvia brew target': ['tempTarget', 'num'],
      'silvia pid heat output': ['heat', 'num'],
      'silvia pump dimmer output': ['pump', 'num'],
      'silvia weight': ['weight', 'num'],
      'silvia scale flow': ['flow', 'num'],
      'silvia shot profile': ['profile', 'text'],
      'silvia brew shot remaining': ['remaining', 'seconds'],
      'silvia brew shot status': ['shotStatus', 'status'],
      'silvia brew shot': ['shotSwitch', 'bool'],
      'silvia power relay': ['powerState', 'bool'],
      'silvia hot water': ['hotWaterState', 'bool'],
      'silvia steam mode': ['steamModeState', 'bool'],
      'silvia water level': ['waterLevelState', 'bool'],
      'silvia steam target': ['steamTargetLive', 'num'],
      'silvia backflush reminder shots': ['backflushReminderLive', 'num0'],
      'silvia scale auto tare': ['scaleAutoTare', 'toggle'],
      'silvia stop brew by weight': ['stopByWeight', 'toggle'],
      'silvia shot diagnostics': ['shotDiagnostics', 'toggle'],
      'silvia pid warmup active': ['warmupStatus', 'status_bool'],
      'silvia shot diagnostic csv ready': ['csvReady', 'status_bool'],
      'silvia adaptive pressure feed forward': ['pressureFeedForward', 'num'],
      'silvia pressure rise rate': ['pressureRiseRate', 'num'],
      'silvia pressure sensor temperature': ['pressureSensorTemp', 'num'],
      'silvia pressure data age': ['pressureDataAge', 'num'],
      'silvia boiler temperature pt100': ['pt100Temperature', 'num'],
      'silvia pt100 data age': ['pt100DataAge', 'num'],
      'silvia pt100 fault count': ['pt100FaultCount', 'num0'],
      'silvia pt100 diagnostic status': ['pt100Status', 'text'],
      'silvia scale data age': ['scaleDataAge', 'num'],
      'silvia scale fault count': ['scaleFaultCount', 'num0'],
      'silvia scale raw diagnostic': ['scaleRawDiagnostic', 'num0'],
      'silvia scale diagnostic status': ['scaleDiagStatus', 'text'],
      'silvia brew temperature feed forward': ['tempFeedForward', 'num'],
      'silvia pid proportional': ['pidP', 'num'],
      'silvia pid integral': ['pidI', 'num'],
      'silvia pid derivative': ['pidD', 'num'],
      'silvia pid result': ['piOutput', 'num'],
      'silvia pid autotune status': ['autotuneStatus', 'text'],
      'silvia backflush status': ['backflushStatus', 'text'],
      'silvia backflush shots': ['backflushShots', 'num0'],
      'silvia lifetime shots': ['lifetimeShots', 'num0'],
      'silvia monthly shots': ['monthlyShots', 'num0'],
      'silvia monthly drink weight': ['monthlyDrinkWeight', 'num0'],
      'silvia coffee grounds used': ['groundsUsed', 'num'],
      'silvia auto off remaining': ['autoOffRemaining', 'num'],
      'silvia reset reason': ['resetReason', 'text'],
      'silvia xdb401 total errors': ['errTotal', 'num0'],
      'silvia xdb401 start errors': ['errStart', 'num0'],
      'silvia xdb401 status read errors': ['errStatus', 'num0'],
      'silvia xdb401 measurement timeouts': ['errTimeout', 'num0'],
      'silvia xdb401 packet read errors': ['errPacket', 'num0'],
      'silvia xdb401 rejected packets': ['errRejected', 'num0'],
      'silvia xdb401 consecutive errors': ['errConsecutive', 'num0'],
      'silvia xdb401 last error': ['errLast', 'text'],
      'silvia free psram': ['freePsram', 'num'],
      'silvia largest psram block': ['largestPsram', 'num'],
      'silvia free internal heap': ['freeInternal', 'num']
    };

    const nums = {
      brewTarget: 'Silvia Brew Target',
      targetWeight: 'Silvia Brew Target Weight',
      shotSeconds: 'Silvia Brew Shot Seconds',
      prePump: 'Silvia Preinfusion Pump Seconds',
      prePause: 'Silvia Preinfusion Pause Seconds',
      prePower: 'Silvia Custom Start Pressure',
      mainPressure: 'Silvia Custom Main Pressure',
      endPressure: 'Silvia Custom End Pressure',
      softInfusionTime: 'Silvia Soft Infusion Time',
      softInfusionStartPower: 'Silvia Soft Infusion Start Power'
    };

    const machineNums = {
      brewTempOffset: 'Silvia Brew Temperature Offset',
      steamTarget: 'Silvia Steam Target',
      autoOff: 'Silvia Auto Off Minutes',
      dose: 'Silvia Coffee Dose Grams'
    };

    const temperatureNums = {
      pidKp: 'Silvia PID KP',
      pidKi: 'Silvia PID KI',
      pidKd: 'Silvia PID KD',
      pidWarmupRange: 'Silvia PID Warmup Range',
      tempFeedForwardMax: 'Silvia Brew Temperature Feed Forward Max'
    };

    const pressureNums = {
      pressureKp: 'Silvia Pressure Control Kp',
      pressureKi: 'Silvia Pressure Control Ki',
      pressureSoftInfusionTime: 'Silvia Soft Infusion Time',
      pressureSoftInfusionStartPower: 'Silvia Soft Infusion Start Power'
    };

    const scaleNums = {
      scaleCalibrationWeight: 'Silvia Scale Calibration Weight'
    };

    const backflushNums = {
      backflushReminder: 'Silvia Backflush Reminder Shots',
      backflushRinseDelay: 'Silvia Backflush Rinse Delay Seconds'
    };

    const switchControls = {
      scaleAutoTare: 'Silvia Scale Auto Tare',
      stopByWeight: 'Silvia Stop Brew By Weight',
      shotDiagnostics: 'Silvia Shot Diagnostics'
    };

    const statusEntities = [
      ['sensor', 'Silvia Adaptive Pressure Feed Forward', 'pressureFeedForward', 'num'],
      ['sensor', 'Silvia Pressure Rise Rate', 'pressureRiseRate', 'num'],
      ['sensor', 'Silvia Pressure Sensor Temperature', 'pressureSensorTemp', 'num'],
      ['sensor', 'Silvia Pressure Data Age', 'pressureDataAge', 'num'],
      ['sensor', 'Silvia Boiler Temperature PT100', 'pt100Temperature', 'num'],
      ['sensor', 'Silvia PT100 Data Age', 'pt100DataAge', 'num'],
      ['sensor', 'Silvia PT100 Fault Count', 'pt100FaultCount', 'num0'],
      ['text_sensor', 'Silvia PT100 Diagnostic Status', 'pt100Status', 'text'],
      ['sensor', 'Silvia Scale Data Age', 'scaleDataAge', 'num'],
      ['sensor', 'Silvia Scale Fault Count', 'scaleFaultCount', 'num0'],
      ['sensor', 'Silvia Scale Raw Diagnostic', 'scaleRawDiagnostic', 'num0'],
      ['text_sensor', 'Silvia Scale Diagnostic Status', 'scaleDiagStatus', 'text'],
      ['sensor', 'Silvia Weight', 'scaleDiagnosticWeight', 'num'],
      ['sensor', 'Silvia Brew Temperature Feed Forward', 'tempFeedForward', 'num'],
      ['sensor', 'Silvia PID Proportional', 'pidP', 'num'],
      ['sensor', 'Silvia PID Integral', 'pidI', 'num'],
      ['sensor', 'Silvia PID Derivative', 'pidD', 'num'],
      ['sensor', 'Silvia PID Result', 'piOutput', 'num'],
      ['binary_sensor', 'Silvia PID Warmup Active', 'warmupStatus', 'status_bool'],
      ['binary_sensor', 'Silvia Shot Diagnostic CSV Ready', 'csvReady', 'status_bool'],
      ['text_sensor', 'Silvia PID Autotune Status', 'autotuneStatus', 'text'],
      ['text_sensor', 'Silvia Backflush Status', 'backflushStatus', 'text'],
      ['text_sensor', 'Silvia Reset Reason', 'resetReason', 'text'],
      ['sensor', 'Silvia Backflush Shots', 'backflushShots', 'num0'],
      ['sensor', 'Silvia Lifetime Shots', 'lifetimeShots', 'num0'],
      ['sensor', 'Silvia Monthly Shots', 'monthlyShots', 'num0'],
      ['sensor', 'Silvia Monthly Drink Weight', 'monthlyDrinkWeight', 'num0'],
      ['sensor', 'Silvia Coffee Grounds Used', 'groundsUsed', 'num'],
      ['sensor', 'Silvia Auto Off Remaining', 'autoOffRemaining', 'num'],
      ['sensor', 'Silvia XDB401 Total Errors', 'errTotal', 'num0'],
      ['sensor', 'Silvia XDB401 Start Errors', 'errStart', 'num0'],
      ['sensor', 'Silvia XDB401 Status Read Errors', 'errStatus', 'num0'],
      ['sensor', 'Silvia XDB401 Measurement Timeouts', 'errTimeout', 'num0'],
      ['sensor', 'Silvia XDB401 Packet Read Errors', 'errPacket', 'num0'],
      ['sensor', 'Silvia XDB401 Rejected Packets', 'errRejected', 'num0'],
      ['sensor', 'Silvia XDB401 Consecutive Errors', 'errConsecutive', 'num0'],
      ['text_sensor', 'Silvia XDB401 Last Error', 'errLast', 'text'],
      ['sensor', 'Silvia Free PSRAM', 'freePsram', 'num'],
      ['sensor', 'Silvia Largest PSRAM Block', 'largestPsram', 'num'],
      ['sensor', 'Silvia Free Internal Heap', 'freeInternal', 'num']
    ];

    const $ = id => document.getElementById(id);
    const clean = id => (id || '').toLowerCase().replace(/[\/_-]+/g, ' ').replace(/^(sensor|number|select|switch|binary sensor) /, '').trim();
    const sleep = ms => new Promise(resolve => setTimeout(resolve, ms));

    const EN_TRANSLATIONS = {"Локальная панель":"Local dashboard","Главная":"Home","Рецепты":"Recipes","История":"History","Настройки":"Settings","Обслуживание":"Maintenance","Диагностика":"Diagnostics","Управление кофемашиной":"Espresso machine control","Включить":"Turn on","Выключить":"Turn off","подключение…":"connecting…","онлайн":"online","нет связи":"offline","Давление":"Pressure","бар":"bar","Основная цель профиля":"Main profile target","Цель сейчас":"Current target","— бар":"— bar","Ожидание":"Idle","Начать пролив":"Start shot","Остановить":"Stop","Температура":"Temperature","Цель":"Target","Факт":"Actual","Секунды":"Seconds","Вес":"Weight","г":"g","Поток":"Flow","г/с":"g/s","Помпа":"Pump","Датчик":"Sensor","Давление, поток и мощность помпы":"Pressure, flow and pump power","Профиль":"Selected profile","Последний пролив":"Last shot","Температурная стабильность":"Temperature stability","Температура воды и выход нагревателя":"Water temperature and heater output","Цель воды":"Water target","Нагрев":"Heat","Сценарий шота":"Shot sequence","Загрузка параметров…":"Loading parameters…","Выбор профиля и параметры напитка.":"Profile selection and drink parameters.","Рецепт":"Recipe","Активный профиль":"Active profile","Температура кофе, °C":"Brew temperature, °C","Доза, г":"Dose, g","Целевой вес, г":"Target weight, g","Время основного пролива, с":"Main extraction time, s","Профиль давления":"Pressure profile","Предсмачивание, с":"Preinfusion, s","Пауза, с":"Pause, s","Давление предсмачивания, бар":"Preinfusion pressure, bar","Основное давление, бар":"Main pressure, bar","Конечное давление, бар":"End pressure, bar","Сохранить как Custom":"Save as Custom","Автоматика веса":"Weight automation","Остановка по весу":"Stop by weight","Остановить пролив по целевому весу и прогнозу потока.":"Stop the shot using target weight and flow prediction.","Целевой вес задаётся в рецепте выше. Время пролива остаётся максимальным защитным ограничением.":"Target weight is set in the recipe above. Shot time remains the maximum safety limit.","История проливов":"Shot history","Локальный архив ESP32 и сохранённая цель каждого пролива.":"Local ESP32 archive with the stored target for each shot.","Архив проливов ESP32":"ESP32 shot archive","Сохранённый пролив":"Saved shot","Архив пуст":"Archive is empty","Обновить":"Refresh","Показать на главной":"Show on Home","После первого пролива здесь появится его локальный паспорт.":"A local shot summary will appear here after the first extraction.","Настройки управления":"Control settings","Температурный PID, регулирование давления и запуск помпы.":"Temperature PID, pressure control and pump startup.","Интерфейс":"Interface","Язык локальной панели сохраняется в этом браузере.":"The dashboard language is saved in this browser.","Язык":"Language","Температурный PID":"Temperature PID","Коэффициенты нагревателя и поведение при прогреве.":"Heater coefficients and warm-up behavior.","Диапазон прогрева, °C":"Warm-up range, °C","Поправка температуры, °C":"Temperature offset, °C","Feed-forward нагрева, максимум %":"Heat feed-forward maximum, %","Сохранить PID":"Save PID","Прогрев без интеграла":"Warm-up without integral","Автонастройка":"Autotune","Регулирование давления":"Pressure control","Коэффициенты замкнутого регулятора и мягкое начало фазы.":"Closed-loop coefficients and soft phase start.","Kp давления":"Pressure Kp","Ki давления":"Pressure Ki","Мягкое наполнение, с":"Soft infusion, s","Начальная мощность, %":"Starting power, %","Сохранить давление":"Save pressure settings","Адаптивный выход":"Adaptive output","Скорость давления":"Pressure rate","бар/с":"bar/s","Режим машины":"Machine mode","Режим PID":"PID mode","Цель пара, °C":"Steam target, °C","Автовыключение, мин":"Auto-off, min","Сохранить":"Save","Сбросить таймер":"Reset timer","До автоотключения":"Until auto-off","мин":"min","Весы, обратная промывка, диагностическая запись и сервис устройства.":"Scale, backflush, diagnostic recording and device service.","Весы":"Scale","Калибровочный вес, г":"Calibration weight, g","Автоматическая тара":"Automatic tare","Тарировать стабильные весы перед следующим проливом.":"Tare stable scales before the next shot.","Тарировать":"Tare","Калибровать":"Calibrate","Калибровка: сначала пустые весы → «Тарировать», затем положить указанный вес → «Калибровать».":"Calibration: empty the scale → “Tare”, then place the specified weight → “Calibrate”.","Обратная промывка":"Backflush","Напоминание, проливов":"Reminder, shots","Пауза перед ополаскиванием, с":"Delay before rinse, s","Сохранить параметры":"Save parameters","Статус":"Status","После промывки":"Since backflush","Всего проливов":"Lifetime shots","Запустить":"Start","Сбросить счётчик":"Reset counter","Диагностика пролива":"Shot diagnostics","Записать следующий пролив":"Record next shot","Синхронный CSV внутренних данных регулятора.":"Synchronized CSV of internal controller data.","CSV готов":"CSV ready","Скачать CSV":"Download CSV","Очистить":"Clear","Устройство":"Device","Причина перезапуска":"Restart reason","Израсходовано кофе":"Coffee used","Перезапустить ESP32":"Restart ESP32","Живые показатели регуляторов, XDB401 и памяти ESP32.":"Live controller, XDB401 and ESP32 memory values.","Давление и помпа":"Pressure and pump","Давление и XDB401":"Pressure and XDB401","Диагностика регулятора давления и датчика XDB401.":"Pressure-controller and XDB401 diagnostics","Регулятор":"Controller","Температура, обмен, пакеты и состояние датчика давления.":"Temperature, communication, packets, and pressure-sensor status","Текущее состояние":"Current status","Рост давления":"Pressure rise","Температура XDB401":"XDB401 temperature","PID результат":"PID output","Прогрев":"Warm-up","XDB401, память и система":"XDB401, memory and system","Всего ошибок":"Total errors","Старт":"Start","Тайм-аут":"Timeout","Пакет":"Packet","Отбраковано":"Rejected","Подряд":"Consecutive","Последняя ошибка":"Last error","Свободно PSRAM":"Free PSRAM","Крупнейший блок PSRAM":"Largest PSRAM block","Внутренний heap":"Internal heap","Состояние датчика":"Sensor health","Скачать последний CSV":"Download latest CSV","Стандартная страница ESPHome":"Standard ESPHome page","Свернуть меню":"Collapse menu","Открыть меню":"Open menu","Разделы":"Sections","Текущий пролив · цель записывается вместе с данными":"Current shot · target is recorded with the data","Последний пролив · сохранённая цель этого пролива":"Last shot · this shot’s stored target","Архивный пролив · цель взята из CSV":"Archived shot · target loaded from CSV","помпа":"pump","нагрев":"heat","с":"s","Ошибка":"Error","Да":"Yes","Нет":"No","Активен":"Active","Пролив":"Brew","Предсмачивание":"Preinfusion","Пауза":"Pause","Готов":"Ready","Включено":"Enabled","Выключено":"Disabled","Команда не выполнена":"Command failed","Питание не изменено":"Power state was not changed","Профиль не применён":"Profile was not applied","Ручные настройки сохранены как Custom":"Manual settings saved as Custom","Ошибка сохранения":"Save failed","Настройки сохранены":"Settings saved","Переключатель не изменён":"Switch was not changed","Режим не изменён":"Mode was not changed","Команда отправлена":"Command sent","Калибровка запущена":"Calibration started","Калибровка не запущена":"Calibration did not start","Архив пока недоступен":"Archive is currently unavailable","профиль не указан":"profile not specified","цель из CSV":"target from CSV","пик":"peak","ошибки XDB401":"XDB401 errors","Не удалось открыть пролив":"Could not open shot","Пролив запущен":"Shot started","Пролив остановлен":"Shot stopped","Кофемашина включена":"Espresso machine turned on","Кофемашина выключена":"Espresso machine turned off","Температурный PID сохранён":"Temperature PID saved","Настройки давления сохранены":"Pressure settings saved","Настройки машины сохранены":"Machine settings saved","Параметры обслуживания сохранены":"Maintenance settings saved","Запустить автонастройку PID? Машина должна быть включена и находиться под наблюдением.":"Start PID autotune? The machine must be powered on and supervised.","Запустить автоматическую обратную промывку? Установи глухую корзину и не оставляй машину без присмотра.":"Start automatic backflush? Install the blind basket and do not leave the machine unattended.","Сбросить счётчик проливов после промывки?":"Reset the shot counter after backflush?","Удалить диагностический CSV из памяти?":"Delete the diagnostic CSV from memory?","Перезапустить ESP32? Управление машиной временно отключится.":"Restart ESP32? Machine control will be temporarily unavailable.","Язык изменён":"Language changed","Питание":"Power","Выключена":"Off","Готова":"Ready","Нагрев пара":"Heating steam","Пар готов":"Steam ready","Подача воды":"Hot water","Ошибка датчика":"Sensor error","Бак воды":"Water tank","Вода есть":"Water available","Мало воды":"Low water","Нет данных":"No data","Промывка":"Backflush","До промывки":"Until cleaning","Проливов всего":"Total shots","Быстрые действия":"Quick actions","Остановить пролив":"Stop shot","Вода":"Water","Пар":"Steam","Вода включена":"Hot water enabled","Вода выключена":"Hot water disabled","Пар включён":"Steam enabled","Пар выключен":"Steam disabled","Напоминание выключено":"Reminder disabled","Настройка профиля с мгновенным предпросмотром.":"Profile editing with instant preview.","Редактор профиля":"Profile editor","Профиль синхронизирован":"Profile synchronized","Параметры пролива":"Shot parameters","Температура кофе":"Brew temperature","Время пролива":"Shot time","Мягкий старт":"Soft start","Стартовая мощность":"Starting power","Точки давления":"Pressure points","Начало":"Start","Основная":"Main","Финиш":"Finish","Напиток и весы":"Drink and scale","Вес напитка":"Beverage weight","Стоп по весу":"Stop by weight","Доза кофе":"Coffee dose","Целевой вес":"Target weight","Время пролива остаётся защитным ограничением.":"Shot time remains a safety limit.","Сбросить изменения":"Reset changes","Сохранить Custom":"Save Custom","Предпросмотр профиля":"Profile preview","Изменения отображаются сразу":"Changes appear instantly","Общее время":"Total time","Исходный профиль":"Original profile","Изменённый профиль":"Edited profile","Текущий профиль":"Current profile","Изменения не сохранены":"Unsaved changes","База":"Base","Базовый профиль":"Base profile","Основной пролив":"Main extraction","Старт помпы":"Pump start","Нет изменений для сброса":"No changes to reset","Изменения сброшены":"Changes reset","сохранено":"saved","Профиль загружен":"Profile loaded","Применение профиля…":"Applying profile…","Перетащите цветные точки давления вверх или вниз. Время меняется кнопками слева.":"Drag the colored pressure points up or down. Change timing with the controls on the left.","График пролива":"Shot graph","Температура и нагрев":"Temperature and heat","ОК":"OK","Пора":"Due","шотов":"shots","Останавливать по весу":"Stop at target weight","Пролив остановится при достижении целевого веса. Время остаётся защитным ограничением.":"The shot stops when the target weight is reached. Time remains a safety limit.","Выбрать профиль":"Select profile","Кривая":"Curve","Редактирование кривой":"Curve editing","Перетаскивайте цветные точки давления вверх или вниз.":"Drag the colored pressure points up or down.","Отмена":"Cancel","Увеличить график":"Expand chart","Уменьшить график":"Collapse chart","Изменения кривой отменены":"Curve changes canceled","Кривая сохранена как Custom":"Curve saved as Custom","Редактировать кривую":"Edit curve","Завершить редактирование":"Finish editing","Нет воды":"No water","Последние 10 минут · в реальном времени":"Last 10 minutes · live","Последние 5 минут · в реальном времени":"Last 5 minutes · live","5 мин":"5 min","10 мин":"10 min","Профиль и этапы выбранного пролива":"Profile and stages of the selected shot","Последние 3 минуты":"Last 3 minutes","Последние 5 минут":"Last 5 minutes","Последние 10 минут":"Last 10 minutes","3 мин":"3 min","Автоотключение":"Auto-off","Автоотключение через":"Auto-off in","Автоотключение выключено":"Auto-off disabled","через":"in","выключено":"disabled","Настроить экран":"Customize screen","Готово":"Done","Сбросить":"Reset","Перетаскивайте крупные блоки за ручку ⠿":"Drag large blocks using the ⠿ handle","Перетащить блок":"Drag block","Расположение сохранено":"Layout saved","Расположение сброшено":"Layout reset","Редактирование профиля":"Profile editing","Архивный пролив · профиль из CSV":"Archived shot · profile from CSV","Редактировать профиль":"Edit profile","Редактирование недоступно во время пролива":"Editing is unavailable during a shot","Профиль сохранён как Custom":"Profile saved as Custom","Изменения профиля отменены":"Profile changes canceled","Параметры и кривая обновляются сразу. Сохранение создаёт Custom.":"Parameters and curve update instantly. Saving creates Custom.","Точки давления также можно перетаскивать прямо на графике.":"Pressure points can also be dragged directly on the chart.","Редактировать точки давления":"Edit pressure points","Перетаскивайте цветные точки прямо на графике.":"Drag the colored points directly on the chart.","Изменение отменено":"Change canceled","Параметр":"Parameter","Перетаскивайте блоки за ⠿ или скрывайте кнопкой справа":"Drag blocks using ⠿ or hide them with the right button","Скрытые блоки":"Hidden blocks","Скрыть блок":"Hide block","Состояние машины":"Machine status","Температурный график":"Temperature chart","Температура и PID":"Temperature and PID","Состояние бойлера за последнюю минуту":"Boiler status over the last minute","Нагрев PID":"PID heating","PID активен":"PID active","Удержание":"Holding","Температура на цели":"Temperature on target","До цели":"To target","Выше цели":"Above target","Ожидание данных":"Waiting for data","Состояние":"Status","График":"Chart","Текущее состояние бойлера":"Current boiler status","Период":"Period","Открыть рецепты":"Open recipes","Готов к проливу":"Ready to brew","Машина выключена":"Machine is off","Стоп":"Stop","Состояние бака воды":"Water tank status","Режим":"Mode","Завершить настройку экрана":"Finish dashboard setup","Состояние и пролив":"Status and brew","Основные показатели машины":"Main machine metrics","Вес пролива":"Brew weight","Счётчик проливов":"Shot counter","Требуется промывка":"Cleaning required","Встроенные профили":"Built-in profiles","Профили кофемашины":"Machine profiles","Мои профили":"My profiles","Сохраняются в этом браузере":"Stored in this browser","Название профиля":"Profile name","Сохранить текущий профиль":"Save current profile","Сохранённых профилей пока нет":"No saved profiles yet","Пользовательские настройки":"Custom settings","Введите название профиля":"Enter a profile name","Профиль сохранён":"Profile saved","Профиль обновлён":"Profile updated","Переименовать профиль":"Rename profile","Новое название профиля":"New profile name","Профиль с таким названием уже существует":"A profile with this name already exists","Профиль переименован":"Profile renamed","Удалить профиль":"Delete profile","Профиль удалён":"Profile deleted","Не удалось сохранить профиль в браузере":"Could not save profile in the browser","Профиль применён":"Profile applied","Вкл":"On","Выкл":"Off","Остановка по весу включена":"Stop by weight is on","Остановка по весу выключена":"Stop by weight is off","Изменения профиля":"Profile changes","Проверьте форму кривой и сохраните её как «Свой».":"Check the curve shape and save it as Custom.","Перетаскивайте точки и границы стадий":"Drag points and stage boundaries","Двойной клик по основному проливу переносит точку «Основная»":"Double-click the main extraction to move the Main point","Набор":"Ramp","Двойной клик доступен в стадии основного пролива":"Double-click is available in the main extraction stage","Есть изменения — введите название и нажмите +":"Changes are ready — enter a name and press +","Профиль сохранён и применён":"Profile saved and applied","Профиль обновлён и применён":"Profile updated and applied","Профиль сохранён, но не применён":"Profile saved but not applied","Сначала измените профиль":"Change the profile first","Перетащить карточку":"Drag card","Время изменяется маркером на графике":"Time is changed with the chart marker","Изменить целевой вес":"Edit target weight","Тяните маркеры этапов вправо или влево":"Drag stage markers left or right","Нажмите «＋ Предсмачивание» или «＋ Пауза», чтобы быстро добавить этап":"Tap + Preinfusion or + Pause to add the stage","Настроить шрифты":"Adjust fonts","Размеры шрифтов":"Font sizes","Изменения применяются сразу и сохраняются в этом браузере":"Changes apply immediately and are saved in this browser","Обычный текст":"Regular text","Заголовки":"Headings","Крупные значения":"Large values","Подписи графиков":"Chart labels","Текст":"Text","Значения":"Values","Размеры шрифтов сброшены":"Font sizes reset","Добавить":"Add","Вернуть размеры Home Assistant":"Restore Home Assistant sizes","Общие размеры":"Global sizes","Базовая типографика всей панели":"Base typography for the whole dashboard","Отдельные окна":"Individual panels","Дополнительный масштаб поверх общих настроек":"Additional scale on top of global settings","Левое меню":"Left menu","Управление машиной":"Machine controls","Показатели":"Metrics","Перенос настроек":"Transfer settings","Скопируйте эту строку и отправьте её мне или вставьте в другом браузере":"Copy this line and send it to me or paste it in another browser","Копировать настройки":"Copy settings","Применить из поля":"Apply from field","Настройки шрифтов":"Font settings","Настройки шрифтов скопированы":"Font settings copied","Скопируйте строку из поля":"Copy the line from the field","Неверный формат настроек":"Invalid settings format","Настройки шрифтов применены":"Font settings applied","Общие размеры и отдельный масштаб каждого окна сохраняются в этом браузере":"Global sizes and each panel scale are saved in this browser","Независимые размеры шрифтов":"Independent font sizes","Каждый блок меняется отдельно. Размеры указаны прямо в пикселях.":"Each panel changes independently. Sizes are shown directly in pixels.","Изменение одного окна больше не влияет на остальные окна.":"Changing one panel no longer affects the other panels.","Настраивается независимо":"Adjusted independently","Заголовок":"Heading","График / мелкие подписи":"Chart / small labels","Сбросить это окно":"Reset this panel","Сбросить всё":"Reset all","Независимые размеры в пикселях":"Independent sizes in pixels","Размеры окна сброшены":"Panel font sizes reset","Скопируйте строку и отправьте её мне — эти размеры можно сделать значениями по умолчанию":"Copy the line and send it to me — these sizes can become the defaults","Изменение одного окна больше не влияет на остальные окна. Текущий набор сохранён как стандартный.":"Changing one panel no longer affects the others. The current set is saved as the default.","Встроенные":"Built-in","Мои":"Mine","Тяните влево или вправо для изменения цели":"Drag left or right to change the target","Тяните влево или вправо для изменения предсмачивания":"Drag left or right to change preinfusion","Тяните влево или вправо для изменения паузы":"Drag left or right to change the pause","Тяните влево или вправо для изменения времени пролива":"Drag left or right to change brew time","Тяните влево или вправо для изменения целевого веса":"Drag left or right to change target weight","Предсмач.":"Preinf.","Поток, г/с":"Flow, g/s","Сохранённые проливы":"Saved shots","Обновить историю":"Refresh history","Проливы":"Shots","Загрузка истории…":"Loading history…","Выберите пролив":"Select a shot","График и показатели появятся здесь.":"The chart and metrics will appear here.","Время":"Duration","Пик давления":"Peak pressure","Цель веса":"Target weight","История пока пуста":"History is empty","После первого пролива здесь появится запись.":"A record will appear here after the first shot.","Профиль не указан":"Profile not specified","Загрузка…":"Loading…","Данные пролива":"Shot data","Загрузка пролива…":"Loading shot…","Читаем график и показатели.":"Reading chart and metrics.","Файл архива недоступен.":"The archive file is unavailable.","Доза":"Dose","Ошибки XDB401":"XDB401 errors","Архив недоступен":"Archive unavailable","Не удалось получить список проливов.":"Could not load the shot list.","Профили и проливы":"Profiles and shots","Выберите запись":"Select a record","Обновить проливы":"Refresh shots","Параметры управления машиной":"Machine control parameters","Категории":"Categories","PID и прогрев":"PID and warmup","Регулятор и наполнение":"Control and infusion","Машина":"Machine","Режим и автовыключение":"Mode and auto-off","Язык панели":"Dashboard language","Нагрев, прогрев и температурная компенсация.":"Heating, warmup and temperature compensation.","Feed-forward, максимум %":"Feed-forward, maximum %","Замкнутый регулятор и мягкое начало пролива.":"Closed-loop control and a gentle shot start.","Рабочий режим, температура пара и таймер питания.":"Operating mode, steam temperature and power timer.","Параметры локальной панели.":"Local dashboard settings.","Выбранный язык сохраняется только в этом браузере.":"The selected language is stored only in this browser.","Весы, промывка, запись пролива и устройство":"Scale, backflush, shot logging and device","4 раздела":"4 sections","Тара и калибровка":"Tare and calibration","Цикл и напоминание":"Cycle and reminder","Запись пролива":"Shot logging","Диагностический CSV":"Diagnostic CSV","Счётчики и перезапуск":"Counters and restart","Тара, автоматическая подготовка и калибровка.":"Tare, automatic preparation and calibration.","Сначала оставьте весы пустыми и нажмите «Тарировать», затем положите указанный вес и нажмите «Калибровать».":"First leave the scale empty and press Tare, then place the specified weight and press Calibrate.","Настройка напоминания и управление автоматическим циклом.":"Reminder settings and automatic cycle control.","Синхронная запись внутренних данных регулятора в CSV.":"Synchronous logging of internal controller data to CSV.","Запись активируется только для следующего запуска помпы.":"Logging is enabled only for the next pump start.","Состояние контроллера и сервисные действия.":"Controller status and service actions.","Регуляторы, датчик давления и память ESP32":"Controllers, pressure sensor and ESP32 memory","3 раздела":"3 sections","Помпа и XDB401":"Pump and XDB401","PID и нагрев":"PID and heating","Ошибки и память":"Errors and memory","Живые значения регулятора давления и датчика XDB401.":"Live pressure controller and XDB401 sensor values.","Текущая температура, составляющие PID и feed-forward.":"Current temperature, PID components and feed-forward.","Ошибки обмена, состояние датчика и память контроллера.":"Communication errors, sensor status and controller memory.","Язык интерфейса":"Interface language","Русский язык":"Russian language","Английский язык":"English language","Профиль пролива":"Shot profile","Профили":"Profiles","Расположение карточки":"Card position","Перетащить верхнюю карточку":"Drag upper card","Изменить ширину верхних карточек":"Resize upper cards","Двойной щелчок — стандартный размер":"Double-click for default size","Изменить ширину нижних карточек":"Resize lower cards","Иконки Пролив / Вода / Пар":"Brew / Water / Steam icons","Круглая кнопка питания":"Round power button","Размеры по умолчанию":"Default sizes","Добавить быстрый ярлык":"Add quick shortcut","Быстрый ярлык":"Quick shortcut","Выберите действие для четвёртой плитки":"Choose an action for the fourth tile","Действие":"Action","Удалить":"Remove","Закрыть":"Close","График температуры":"Temperature chart","Отступ между верхним и нижним блоками":"Gap between upper and lower blocks","из":"of","проливов":"shots","Счётчик":"Counter","Высота температурного графика":"Temperature chart height","Ширина левой / правой карточки":"Left / right card width","Ширина управление / давление / температура":"Controls / pressure / temperature width","Манометр давления от 0 до 12 бар":"Pressure gauge from 0 to 12 bar","Проверяем XDB401…":"Checking XDB401…","Получаем состояние датчика и счётчики обмена.":"Loading sensor state and communication counters.","Ошибок подряд:":"Consecutive errors:","Последняя:":"Latest:","Последний диагностический CSV":"Latest diagnostic CSV","Проверяем запись последнего пролива…":"Checking the latest recorded shot…","Обновить анализ":"Refresh analysis","После анализа здесь появится понятное объяснение остановки помпы и ошибок XDB401.":"A clear explanation of pump shutdown and XDB401 errors will appear here.","Срабатывание защиты":"Protection triggered","Возраст данных":"Data age","Восстановление связи":"Connection recovery","Причина":"Cause","Накопительная статистика XDB401":"Cumulative XDB401 statistics","Загружаем общие счётчики ошибок с момента запуска контроллера.":"Loading cumulative error counters since controller startup.","Технические счётчики":"Technical counters","Текущие значения":"Current values","XDB401 работает нормально":"XDB401 is working normally","Сейчас данные давления поступают. Накопленные ошибки были раньше и не образуют текущую серию.":"Pressure data is currently arriving. Accumulated errors happened earlier and do not form a current sequence.","Данные давления поступают, ошибок связи подряд нет.":"Pressure data is arriving and there are no consecutive communication errors.","XDB401 отвечает с ошибками":"XDB401 is responding with errors","Появилась короткая серия ошибок обмена. Контроллер пока продолжает работу, но связь с датчиком нестабильна.":"A short sequence of communication errors appeared. The controller is still operating, but the sensor link is unstable.","Связь с XDB401 потеряна":"Connection to XDB401 lost","Помпа остановлена защитой: контроллер не получает надёжные данные давления.":"The pump was stopped by protection because the controller is not receiving reliable pressure data.","Контроллер не получает надёжные данные давления. При проливе защита может остановить помпу.":"The controller is not receiving reliable pressure data. During a shot, protection may stop the pump.","Нет текущей ошибки":"No current error","запуск измерения":"measurement start","чтение статуса":"status read","тайм-аут измерения":"measurement timeout","чтение пакета":"packet read","отбракованные пакеты":"rejected packets","Сейчас идёт серия ошибок XDB401. Ниже показаны накопительные счётчики с момента запуска ESP32.":"An XDB401 error sequence is active. The counters below are cumulative since ESP32 startup.","Ошибки XDB401 уже фиксировались. Эти значения накопительные и относятся ко всему времени работы после запуска ESP32.":"XDB401 errors have been recorded. These values are cumulative for the entire uptime since ESP32 startup.","После запуска ESP32 ошибок обмена с XDB401 пока не зарегистрировано.":"No XDB401 communication errors have been recorded since ESP32 startup.","Чаще всего":"Most often","Ошибок нет":"No errors","ош. запуска":"start errors","ош. статуса":"status errors","тайм-аутов":"timeouts","ош. пакета":"packet errors","отбраковано":"rejected","свежие данные давления не поступали":"fresh pressure data was not received","CSV пуст или не распознан":"CSV is empty or unrecognized","В последнем файле нет строк, которые можно использовать для диагностики.":"The latest file contains no rows that can be used for diagnostics.","Защитное отключение не найдено":"No protective shutdown found","Во время последнего пролива XDB401 зарегистрировал ошибки, но данные восстановились до срабатывания защиты и помпа не была принудительно остановлена.":"XDB401 recorded errors during the latest shot, but data recovered before protection triggered and the pump was not forcibly stopped.","Последний записанный пролив прошёл без потери связи с XDB401 и без остановки помпы защитой.":"The latest recorded shot completed without losing XDB401 communication or a protective pump shutdown.","не зафиксировано":"not recorded","Обнаружено защитное отключение":"Protective shutdown detected","На":"At","контроллер потерял надёжные данные XDB401 и остановил помпу защитой SENSOR_FAIL. Максимальный возраст данных составил":"the controller lost reliable XDB401 data and stopped the pump with SENSOR_FAIL protection. Maximum data age was","Ошибок подряд":"Consecutive errors","В последнем CSV обнаружена потеря XDB401, но нулевой выход помпы в строках сбоя не найден.":"The latest CSV shows an XDB401 loss, but zero pump output was not found in the failure rows.","Последний CSV недоступен":"Latest CSV unavailable","Запишите диагностический пролив, после чего здесь появится автоматический разбор связи XDB401 и остановки помпы.":"Record a diagnostic shot and an automatic analysis of XDB401 communication and pump shutdown will appear here.","мс":"ms","Ошибки и состояние системы":"Errors and system status","Ошибки датчика отделены от памяти и состояния ESP32.":"Sensor errors are separated from ESP32 memory and status.","Ошибки датчика давления":"Pressure sensor errors","XDB401: обмен, пакеты и текущее состояние связи.":"XDB401 communication, packets, and current link status.","Состояние:":"Status:","Технические счётчики XDB401":"XDB401 technical counters","Старт измерения":"Measurement start","Чтение статуса":"Status read","Тайм-аут измерения":"Measurement timeout","Чтение пакета":"Packet read","Отбраковано пакетов":"Rejected packets","Последняя ошибка XDB401":"Latest XDB401 error","Система":"System","Память и сервисные данные ESP32.":"ESP32 memory and service data.","ESP32 и память":"ESP32 and memory","Ресурсы контроллера отдельно от ошибок датчика.":"Controller resources separated from sensor errors.","Свободный внутренний heap":"Free internal heap","Показатели памяти относятся к ESP32 и не являются ошибками XDB401.":"Memory values belong to the ESP32 and are not XDB401 errors.","Длительность пролива":"Shot duration","Линии графика":"Chart lines","Толщина и прозрачность":"Width and transparency","Толщина":"Width","Прозрачность":"Transparency","Линии по умолчанию":"Reset lines","Настройки линий графика сброшены":"Chart line settings reset","Настройки линий":"Line settings","Цвет":"Color","Цвет, толщина и прозрачность":"Color, width and transparency","По умолчанию":"Defaults","Настроить линии графика":"Configure chart lines","Границы этапов":"Stage boundaries","PI-регулятор управляет мощностью помпы по фактическому давлению и динамике его роста.":"The PI controller adjusts pump power from measured pressure and its rate of change.","PI-регулятор давления":"Pressure PI controller","Поддерживает давление профиля, изменяя мощность помпы по ошибке давления. Настройки действуют для всех профилей.":"Maintains profile pressure by adjusting pump power from pressure error. These settings apply to all profiles.","Определяет, насколько сильно помпа реагирует на текущую разницу между целью и фактическим давлением.":"Controls how strongly the pump reacts to the current difference between target and measured pressure.","Убирает устойчивое отклонение от цели и помогает регулятору подстроиться под сопротивление кофейной таблетки.":"Removes steady pressure error and helps the controller adapt to coffee-puck resistance.","Мягкое начало пролива":"Soft shot start","Общая плавная оболочка запуска помпы. Это не предсмачивание профиля, а ограничение резкого старта для любого профиля.":"A shared gentle pump-start envelope. This is not profile preinfusion; it limits abrupt startup for any profile.","Длительность плавного старта":"Soft-start duration","За это время допустимая мощность помпы увеличивается постепенно. Значение 0 с отключает мягкий старт.":"During this time the allowed pump power rises gradually. A value of 0 s disables soft start.","Начальная мощность помпы":"Initial pump power","Стартовая граница мощности, с которой начинается плавный разгон перед обычным регулированием давления.":"The starting power limit from which the smooth ramp begins before normal pressure control.","Состояние регулятора":"Controller status","Живые диагностические значения. Они рассчитываются автоматически и здесь не настраиваются.":"Live diagnostic values. They are calculated automatically and are not configured here.","Адаптивная коррекция помпы":"Adaptive pump correction","Текущая автоматическая добавка к управлению помпой с учётом поведения давления.":"Current automatic pump-control correction based on pressure behavior.","Насколько быстро прямо сейчас изменяется давление; регулятор использует динамику, чтобы заранее тормозить разгон.":"How quickly pressure is changing right now; the controller uses this trend to brake the rise early.","Сохранить настройки давления":"Save pressure settings"};

    Object.assign(EN_TRANSLATIONS, {
      "Хранятся на кофемашине":"Stored on the espresso machine",
      "Хранилище профилей временно недоступно":"Profile storage is temporarily unavailable",
      "Не удалось загрузить профили с кофемашины":"Could not load profiles from the espresso machine",
      "Не удалось сохранить профиль на кофемашине":"Could not save the profile on the espresso machine",
      "Не удалось переименовать профиль на кофемашине":"Could not rename the profile on the espresso machine",
      "Не удалось удалить профиль с кофемашины":"Could not delete the profile from the espresso machine",
      "На кофемашине уже сохранено 30 профилей":"The espresso machine already has 30 saved profiles",
      "Пояснения":"Help",
      "Скрыть пояснения":"Hide help",
      "Показать пояснение":"Show explanation",
      "Машина":"Machine",
      "Режим нагрева, температура пара и автоматическое отключение.":"Heating mode, steam temperature and automatic shutoff.",
      "Режим нагрева":"Heating mode",
      "Выбирает рабочую логику нагрева: температура заваривания или температура пара.":"Selects the heating logic: brew temperature or steam temperature.",
      "Brew использует цель заваривания, Steam — отдельную целевую температуру пара.":"Brew uses the brew target; Steam uses a separate steam target.",
      "Пар":"Steam",
      "Отдельная целевая температура бойлера для режима пара.":"A separate boiler target temperature for steam mode.",
      "Цель пара":"Steam temperature",
      "Цель PID, к которой нагревается бойлер после включения режима Steam.":"PID target the boiler heats toward after Steam mode is enabled.",
      "Автоотключение":"Auto shutoff",
      "Защита от оставленной включённой машины и лишнего нагрева в простое.":"Helps prevent the machine from being left heating unnecessarily while idle.",
      "Отключить через":"Shut off after",
      "Время бездействия до автоматического отключения машины.":"Idle time before the machine shuts off automatically.",
      "Текущий остаток таймера. Сбрасывается активностью машины или кнопкой ниже.":"Current timer remaining. It resets with machine activity or the button below.",
      "Сохранить настройки машины":"Save machine settings"
    });

    Object.assign(EN_TRANSLATIONS, {
      "Основные настройки":"Main settings",
      "Основные параметры, которые обычно меняют при эксплуатации.":"Everyday machine parameters that are commonly adjusted.",
      "Поправка воды":"Brew-water temperature offset",
      "Датчик стоит на бойлере. Эта поправка связывает его температуру с расчётной температурой воды при заваривании.":"The sensor is mounted on the boiler. This offset relates boiler temperature to the estimated brew-water temperature.",
      "Цель пара":"Steam temperature",
      "Целевая температура бойлера после включения режима пара.":"Boiler target temperature after steam mode is enabled.",
      "Автоотключение":"Auto-off",
      "Время бездействия до автоматического выключения кофемашины.":"Idle time before the machine powers off automatically.",
      "Осталось:":"Remaining:",
      "Доза кофе":"Coffee dose",
      "Количество сухого кофе на один пролив. Используется для учёта расхода кофе.":"Dry coffee dose per shot. Used for coffee-consumption accounting.",
      "PID и прогрев":"PID and warmup",
      "PID нагревателя, прогрев и компенсация температуры во время пролива.":"Heater PID, warmup and temperature compensation during extraction.",
      "Компенсация при проливе":"Shot heat compensation",
      "Дополнительный нагрев помогает компенсировать приток холодной воды во время заваривания.":"Additional heating helps compensate for incoming cold water during extraction.",
      "Ограничивает дополнительную мощность нагрева во время пролива. Это не PID-коэффициент.":"Limits additional heater power during extraction. This is not a PID coefficient.",
      "Сохранён":"Saved",
      "Обновлён":"Updated"
    });
    Object.assign(EN_TRANSLATIONS, {"Температура и PID бойлера": "Boiler temperature and PID", "Датчик на бойлере, прогрев и расчёт температуры воды при заваривании.": "Boiler-mounted sensor, warmup and estimated brew-water temperature.", "PID бойлера": "Boiler PID", "Коэффициенты регулятора нагрева.": "Heating controller coefficients.", "Автонастройка PID": "PID autotune", "Запустить автонастройку": "Start autotune", "Интеграл PID удерживается на нуле до приближения к цели.": "The PID integral is held at zero until the temperature approaches the target.", "Интеграл включается за, °C": "Enable integral within, °C", "Ограничение интеграла": "Integral hold", "Температура заваривания": "Brew temperature", "Поправка между датчиком на бойлере и расчётной температурой воды, плюс упреждение нагрева во время пролива.": "Offset between the boiler sensor and estimated brew-water temperature, plus predictive heating during extraction.", "Поправка воды, °C": "Brew-water temperature offset, °C", "Feed-forward нагрева, максимум %": "Heat feed-forward maximum, %", "Сохранить настройки температуры": "Save temperature settings", "Настройки температуры сохранены": "Temperature settings saved", "Не запущена": "Not started", "Выполняется": "Running", "Завершена": "Completed", "Активно": "Active"});
    Object.assign(EN_TRANSLATIONS, {
      "Три коэффициента регулируют мощность нагревателя по температуре датчика на бойлере.": "Three coefficients control heater power from the boiler temperature sensor.",
      "Сила реакции на текущую ошибку температуры.": "Reaction strength to the current temperature error.",
      "Убирает постоянное небольшое отклонение от цели.": "Removes a small persistent offset from the target.",
      "Сдерживает разгон при быстром приближении к цели.": "Damps the response when temperature approaches the target quickly.",
      "Автоматически подбирает KP, KI и KD. Запускать на включённой машине под наблюдением.": "Automatically tunes KP, KI and KD. Run only with the machine powered on and supervised.",
      "Защищает PID от накопления интеграла, пока бойлер ещё далеко от целевой температуры.": "Prevents integral wind-up while the boiler is still far from target temperature.",
      "Зона включения интеграла": "Integral enable zone",
      "I-часть включается, когда до цели остаётся меньше этого значения. Например: 93 °C и 10 °C → примерно с 83 °C.": "The I term enables when the remaining distance to target is smaller than this value. Example: 93 °C and 10 °C → about 83 °C.",
      "Интеграл PID сейчас": "PID integral now",
      "Меняется автоматически: на далёком прогреве временно отключён, возле цели работает.": "Changes automatically: temporarily disabled during early warm-up and active near the target.",
      "Отключён на прогреве": "Disabled for warm-up",
      "Работает": "Active",
      "Температура воды при заваривании": "Brew-water temperature",
      "Параметры, которые связывают температуру бойлера с водой на выходе и нагревом во время пролива.": "Parameters linking boiler temperature to outlet water and heating during extraction.",
      "Поправка воды": "Water offset",
      "Вычитается из температуры датчика на бойлере для расчёта температуры воды на выходе.": "Subtracted from the boiler sensor temperature to estimate outlet water temperature.",
      "Feed-forward максимум": "Feed-forward maximum",
      "Ограничивает дополнительный нагрев во время пролива для компенсации притока холодной воды.": "Limits additional heating during extraction to compensate for incoming cold water.",
      "Запустить": "Start"
    });
    Object.assign(EN_TRANSLATIONS, {
      "Оценка пролива":"Shot assessment",
      "Подробнее":"Details",
      "Скрыть":"Hide",
      "Рекомендация по помолу":"Grind recommendation",
      "Уверенность датчика":"Sensor confidence",
      "Средняя ошибка давления":"Mean pressure error",
      "Средний поток":"Average flow"
    });
    Object.assign(EN_TRANSLATIONS, {
      "Связь, данные и защита":"Communication, data, and protection",
      "Датчик давления XDB401":"XDB401 pressure sensor",
      "Разбор ошибок обмена, качества данных и защитных остановок.":"Communication errors, data quality, and protective shutdowns.",
      "После анализа здесь появится вывод о связи XDB401 и работе защиты помпы.":"The analysis will explain XDB401 communication and pump protection behavior.",
      "Защита помпы":"Pump protection",
      "Событие":"Event",
      "Макс. возраст данных":"Maximum data age",
      "Макс. ошибок подряд":"Maximum consecutive errors",
      "Источник ошибки":"Error source",
      "Состояние сейчас":"Current status",
      "Связь с датчиком":"Sensor communication",
      "Проверяем связь XDB401…":"Checking XDB401 communication…",
      "Возраст данных:":"Data age:",
      "Температура:":"Temperature:",
      "Сводка после запуска ESP32":"Summary since ESP32 startup",
      "Счётчики ошибок XDB401":"XDB401 error counters",
      "Связь стабильна":"Communication is stable",
      "Сейчас датчик отвечает без ошибок. Накопленные сбои были раньше; их причина видна в последнем CSV и счётчиках ниже.":"The sensor is responding without errors now. Earlier failures are explained by the latest CSV and the counters below.",
      "Данные давления свежие, ошибок обмена подряд нет.":"Pressure data is fresh and there are no consecutive communication errors.",
      "Нет свежих данных XDB401":"No fresh XDB401 data",
      "Последнее значение давления старше 1 секунды. Во время пролива защита SENSOR_FAIL должна обнулить выход помпы.":"The latest pressure value is older than one second. During a shot, SENSOR_FAIL protection should force pump output to zero.",
      "Связь нестабильна":"Communication is unstable",
      "Идёт короткая серия ошибок обмена. При четырёх ошибках подряд защита SENSOR_FAIL остановит помпу.":"A short communication-error sequence is active. Four consecutive errors will trigger SENSOR_FAIL pump protection.",
      "Связь потеряна":"Communication is lost",
      "Защита SENSOR_FAIL остановила помпу: контроллер не получает надёжные данные давления.":"SENSOR_FAIL protection stopped the pump because reliable pressure data is unavailable.",
      "Контроллер не получает надёжные данные давления. Pressure-профиль нельзя безопасно продолжать.":"Reliable pressure data is unavailable, so the pressure profile cannot continue safely.",
      "Защита SENSOR_FAIL не сработала":"SENSOR_FAIL protection was not triggered",
      "Не сработала":"Not triggered",
      "Ошибок за пролив":"Errors during shot",
      "До порога защиты":"Before protection threshold",
      "Не требовалось":"Not required",
      "Ошибок не было":"No errors",
      "Момент отключения":"Shutdown time",
      "Автоматика весов":"Scale automation",
      "Тарировать перед проливом":"Tare before shot",
      "Перед запуском пролива автоматически обнуляет весы, когда показания стабильны.":"Automatically tares the scales before a shot starts once the reading is stable."
    });
    Object.assign(EN_TRANSLATIONS, {
      "Лаборатория":"Lab",
      "Сбор и строгая проверка данных для будущей модели давления":"Collection and strict validation of data for the future pressure model",
      "Сбор данных для модели":"Model data collection",
      "Сохраняет полный CSV каждого пролива и проверяет надёжность XDB401.":"Stores the full CSV for every shot and validates XDB401 reliability.",
      "Готовность набора":"Dataset readiness",
      "Получаем оценки сохранённых проливов.":"Loading saved-shot assessments.",
      "Чистые проливы":"Clean shots",
      "Пригодные окна":"Usable windows",
      "Частичные":"Partial",
      "Отклонены":"Rejected",
      "Профили":"Profiles",
      "Ошибки XDB401":"XDB401 failures",
      "Хранилище LittleFS":"LittleFS storage",
      "Выгрузить пригодные":"Export eligible",
      "Выгрузить все собранные":"Export all collected",
      "Проверенные записи":"Validated records",
      "Звезда защищает запись от автоматического удаления.":"The star protects a record from automatic deletion.",
      "Обновить набор":"Refresh dataset",
      "Загрузка записей…":"Loading records…",
      "Данных пока нет":"No data yet",
      "Ни одного пролива для модели ещё не сохранено.":"No model-training shots have been saved yet.",
      "Набор собирается":"Dataset is being collected",
      "Данных достаточно для пробной модели":"Enough data for a prototype model",
      "Высокая готовность":"High readiness",
      "Проблемы с данными XDB401":"XDB401 data problems",
      "Чистых проливов":"Clean shots",
      "из 50":"of 50",
      "Профилей в чистом наборе":"Profiles in clean dataset",
      "Сбор включён":"Collection enabled",
      "Сбор выключен":"Collection disabled",
      "Не удалось изменить режим сбора":"Could not change collection mode",
      "Чистый":"Clean",
      "Частичный":"Partial",
      "Брак данных":"Rejected",
      "Не для набора":"Not in dataset",
      "окон":"windows",
      "Закрепить":"Pin",
      "Открепить":"Unpin",
      "Не удалось изменить закрепление":"Could not change pin state",
      "Нет pressure-фазы":"No pressure phase",
      "Слишком короткая запись":"Recording is too short",
      "Ошибки обмена XDB401":"XDB401 communication errors",
      "Некорректные отсчёты давления":"Invalid pressure samples",
      "Устаревшие данные давления":"Stale pressure data",
      "Сработал SENSOR_FAIL":"SENSOR_FAIL triggered",
      "Ошибки XDB401 подряд":"Consecutive XDB401 errors",
      "Пропуски временной шкалы":"Timeline gaps",
      "Неправдоподобный скачок давления":"Implausible pressure jump",
      "Нет полных обучающих окон":"No complete training windows",
      "Запись прошла строгую проверку XDB401":"The recording passed strict XDB401 validation",
      "Подходят только отдельные непрерывные окна":"Only isolated continuous windows are usable",
      "Запись исключена из обучения":"The recording is excluded from training",
      "свободно":"free",
      "Примерно поместится ещё":"Estimated remaining capacity",
      "полных проливов":"full shots",
      "Готовим архив…":"Preparing archive…",
      "Скачиваем":"Downloading",
      "Архив готов":"Archive is ready",
      "Нет подходящих записей для выгрузки":"No eligible records to export",
      "Не удалось выгрузить набор":"Could not export dataset"
    });
    Object.assign(EN_TRANSLATIONS, {
      'Не создан':'Not created',
      'Измените любой профиль и нажмите «Применить»':'Edit any profile and press Apply'
    });

    const I18N_MESSAGES = {
      ru: {
        profileApplied: profile => 'Профиль ' + profile + ' применён',
        modeApplied: mode => 'Режим ' + mode + ' применён',
        shotNumber: id => 'Пролив #' + id,
        localSaved: count => 'Локально сохранено: ' + count,
        archiveTitle: (id, profile) => 'Архив #' + id + ' · ' + profile + ' · цель из CSV',
        archiveLoaded: id => 'Пролив #' + id + ' загружен'
      },
      en: {
        profileApplied: profile => 'Profile ' + profile + ' applied',
        modeApplied: mode => 'Mode ' + mode + ' applied',
        shotNumber: id => 'Shot #' + id,
        localSaved: count => 'Saved locally: ' + count,
        archiveTitle: (id, profile) => 'Archive #' + id + ' · ' + profile + ' · target from CSV',
        archiveLoaded: id => 'Shot #' + id + ' loaded'
      }
    };
    const PROFILE_LABELS = {
      ru: { 'Classic': 'Классика', 'Lever': 'Рычаг', 'Slayer Style': 'Slayer', 'Bloom': 'Блум', 'Funnel': 'Воронка', 'Custom': 'Свой' },
      en: { 'Classic': 'Classic', 'Lever': 'Lever', 'Slayer Style': 'Slayer Style', 'Bloom': 'Bloom', 'Funnel': 'Funnel', 'Custom': 'Custom' }
    };
    const PID_MODE_LABELS = {
      ru: { 'Brew': 'Кофе', 'Steam': 'Пар' },
      en: { 'Brew': 'Brew', 'Steam': 'Steam' }
    };
    let availableProfileOptions = [];
    let homeProfileApplying = false;
    let userProfiles = [];
    let userProfilesLoadError = false;
    let activeUserProfileId = '';
    let userProfileApplying = false;
    let userProfileStorageBusy = false;
    let currentLanguage = 'ru';
    const i18nTextNodes = [];
    const i18nAttributes = [];

    function tr(value) {
      if (value === null || value === undefined) return value;
      const text = String(value);
      return currentLanguage === 'en' ? (EN_TRANSLATIONS[text] || text) : text;
    }

    function message(key, ...args) {
      const group = I18N_MESSAGES[currentLanguage] || I18N_MESSAGES.ru;
      const value = group[key] || I18N_MESSAGES.ru[key];
      return typeof value === 'function' ? value(...args) : value;
    }

    function profileLabel(value) {
      const raw = String(value ?? '');
      return (PROFILE_LABELS[currentLanguage] && PROFILE_LABELS[currentLanguage][raw]) || raw;
    }

    function pidModeLabel(value) {
      const raw = String(value ?? '');
      return (PID_MODE_LABELS[currentLanguage] && PID_MODE_LABELS[currentLanguage][raw]) || raw;
    }

    function localizeDynamicText(value) {
      if (value === null || value === undefined || value === '') return '—';
      const raw = String(value);
      if (currentLanguage === 'ru') {
        return raw
          .replace(/\bSlayer Style\b/g, profileLabel('Slayer Style'))
          .replace(/\bSlayer\b/g, profileLabel('Slayer Style'))
          .replace(/\bBloom\b/g, profileLabel('Bloom'))
          .replace(/\bFunnel\b/g, profileLabel('Funnel'))
          .replace(/\bClassic\b/g, profileLabel('Classic'))
          .replace(/\bLever\b/g, profileLabel('Lever'))
          .replace(/\bCustom\b/g, profileLabel('Custom'));
      }
      const direct = EN_TRANSLATIONS[raw];
      if (direct) return direct;
      const replacements = [
        ['Предсмачивание', 'Preinfusion'], ['Пауза', 'Pause'], ['Основной пролив', 'Main extraction'],
        ['Основное давление', 'Main pressure'], ['Конечное давление', 'End pressure'],
        ['Пролив', 'Shot'], ['Готов', 'Ready'], ['Ожидание', 'Idle'], ['Ошибка', 'Error'],
        ['бар/с', 'bar/s'], ['бар', 'bar'], ['г/с', 'g/s'], [' мс', ' ms'], [' с', ' s'], [' г', ' g']
      ];
      return replacements.reduce((text, pair) => text.split(pair[0]).join(pair[1]), raw);
    }

    function captureI18nDom() {
      const walker = document.createTreeWalker(document.body, NodeFilter.SHOW_TEXT);
      while (walker.nextNode()) {
        const node = walker.currentNode;
        const parent = node.parentElement;
        if (!parent || parent.tagName === 'SCRIPT' || parent.tagName === 'STYLE' || parent.id) continue;
        const key = node.nodeValue.trim();
        if (!key) continue;
        i18nTextNodes.push({ node, raw: node.nodeValue, key });
      }
      document.querySelectorAll('[aria-label],[title],[placeholder]').forEach(node => {
        ['aria-label', 'title', 'placeholder'].forEach(name => {
          if (node.hasAttribute(name)) i18nAttributes.push({ node, name, raw: node.getAttribute(name) });
        });
      });
    }

    function translateStaticDom() {
      i18nTextNodes.forEach(item => {
        const leading = item.raw.match(/^\s*/)?.[0] || '';
        const trailing = item.raw.match(/\s*$/)?.[0] || '';
        item.node.nodeValue = leading + tr(item.key) + trailing;
      });
      i18nAttributes.forEach(item => item.node.setAttribute(item.name, tr(item.raw)));
    }

    function relabelSelectOptions() {
      ['profileSelect','homeProfileSelect'].forEach(id => {
        const select = $(id);
        if (select) [...select.options].forEach(option => { option.textContent = profileLabel(option.value); });
      });
      const pidMode = $('pidMode');
      if (pidMode) [...pidMode.options].forEach(option => { option.textContent = pidModeLabel(option.value); });
    }

    function applyLanguage() {
      document.documentElement.lang = currentLanguage;
      translateStaticDom();

      const ruButton = $('languageRuButton');
      const enButton = $('languageEnButton');

      if (ruButton) {
        const active = currentLanguage === 'ru';
        ruButton.classList.toggle('active', active);
        ruButton.setAttribute('aria-pressed', String(active));
      }

      if (enButton) {
        const active = currentLanguage === 'en';
        enButton.classList.toggle('active', active);
        enButton.setAttribute('aria-pressed', String(active));
      }

      const select = $('languageSelect');
      if (select) select.value = currentLanguage;
      relabelSelectOptions();
      syncProfileControls(
        currentProfileName
      );
      updateHomeProfileRail(
        availableProfileOptions,
        currentProfileName
      );
      showPower(machineOn);
      updatePressureTarget();
      updateHomeDashboard();
      updateQuickActions();
      updateRecipeProfileUi();
      updateRecipeDescription();
      updateShotScenario();
      setTemperatureWindow(tempWindowSeconds);
      updateChartControls();
      renderUserProfiles();
      renderHomeArchiveList();
      renderHomeArchiveSummary(
        archiveSelectedSummary
      );
      renderTrainingDataset();

      if ($('homeProfilesTabLabel')) {
        $('homeProfilesTabLabel').textContent =
          tr('Профили');
      }

      if ($('homeShotsTabLabel')) {
        $('homeShotsTabLabel').textContent =
          tr('Проливы');
      }

      updateShotPanelHandleLabels();
      updateShotProfileResizeLabel();
      updateHomeTopPanelHandleLabels();
      updateHomeTopResizeLabel();

      const shotStatusNode =
        $('shotStatus');

      if (shotStatusNode) {
        if (shotRunning) {
          updateShotPhaseStatus();
        } else {
          const rawStatus =
            shotStatusNode.dataset.rawValue ||
            'Ожидание';

          shotStatusNode.textContent =
            localizeDynamicText(rawStatus);
        }
      }

      syncHomeShotRailUi();
      health();
      draw();
      drawTemp();
      drawRecipePreview();
    }

    function setLanguage(language, silent = false) {
      currentLanguage = language === 'en' ? 'en' : 'ru';
      try { localStorage.setItem('silvia-language', currentLanguage); } catch (_) {}
      applyLanguage();
      if (!silent) toast(tr('Язык изменён'));
    }

    function initLanguage() {
      captureI18nDom();
      let saved = 'ru';
      try { saved = localStorage.getItem('silvia-language') || 'ru'; } catch (_) {}
      setLanguage(saved, true);
    }


    const recipeFieldIds = ['brewTarget','targetWeight','shotSeconds','prePump','prePause','prePower','mainPressure','endPressure','softInfusionTime','softInfusionStartPower'];
    const homeQuickFieldMeta = {
      brewTarget:{label:'Цель',unit:'°C',step:0.5,min:80,max:105,pixelsPerStep:8},
      prePump:{label:'Предсмачивание',unit:'с',step:0.5,min:0,max:30,pixelsPerStep:8},
      prePause:{label:'Пауза',unit:'с',step:0.5,min:0,max:30,pixelsPerStep:8},
      shotSeconds:{label:'Время пролива',unit:'с',step:1,min:1,max:240,pixelsPerStep:7},
      targetWeight:{label:'Целевой вес',unit:'г',step:0.5,min:0,max:300,pixelsPerStep:8}
    };
    let recipeBaselineCfg = null;
    let recipeBaseProfile = '';
    let recipeDirty = false;
    let recipeApplying = false;
    let recipeControlPoints = [];
    let recipeHoverKey = '';
    let recipeDragKey = '';
    let recipeClassicLinkedDrag = false;
    let recipeDragPointerId = null;
    let advancedPhaseDraft = null;
    let advancedPhaseSelected = -1;
    let advancedPhaseEditorOpen = false;
    let advancedPreinfusionRampMode = false;
    let profilePhaseEditMode = false;
    let profilePhaseBaseline = null;
    let activeCustomPhaseSpec = '';
    let activeCustomPhaseBase = '';
    let customProfileAvailable = false;

    function customDraftIsAvailable() {
      return !!(
        mainCurveDirty ||
        recipeDirty ||
        profilePhaseEditMode ||
        mainCurveEditing ||
        currentProfileName === 'Custom'
      );
    }

    let mainCurveEditing = false;
    let mainCurveDirty = false;
    let mainCurveBaselineCfg = null;
    let mainCurveBaselineDirty = false;
    let mainCurveControlPoints = [];
    let mainCurveHoverKey = '';
    let mainCurveDragKey = '';
    let mainCurveDragPointerId = null;
    let mainCurveDragStartX = 0;
    let mainCurveDragStartY = 0;
    let mainCurveDragMoved = false;
    let mainCurveDragStartConfig = null;
    let mainCurveClassicLinkedDrag = false;
    let mainCurveDragSecondsPerPixel = 0.1;
    const SHOT_LINE_SETTINGS_KEY =
      'silvia-shot-line-settings-v1';

    const SHOT_LINE_DEFAULTS = {
      profile:{
        color:'#ffd166',
        width:2.6,
        transparency:0
      },
      pressure:{
        color:'#54a9ff',
        width:2.2,
        transparency:0
      },
      flow:{
        color:'#4bd18b',
        width:1.8,
        transparency:0
      },
      pump:{
        color:'#9a72ef',
        width:1.4,
        transparency:65
      },
      stages:{
        color:'#8795a8',
        width:1.2,
        transparency:35
      }
    };

    let shotLineSettings =
      Object.fromEntries(
        Object.entries(
          SHOT_LINE_DEFAULTS
        ).map(([key, value]) => [
          key,
          {...value}
        ])
      );

    let shotLineQuickBaseline = null;

    const SHOT_LINE_VISIBILITY_KEY =
      'silvia-shot-line-visibility-v1';

    const SHOT_LINE_VISIBILITY_DEFAULTS = {
      profile:true,
      pressure:true,
      flow:true,
      pump:true
    };

    let shotLineVisibility = {
      ...SHOT_LINE_VISIBILITY_DEFAULTS
    };

    function shotLineVisible(key) {
      return shotLineVisibility[key] !== false;
    }

    function syncShotLineVisibilityControls() {
      document
        .querySelectorAll('[data-shot-line-visibility]')
        .forEach(button => {
          const key = button.dataset.shotLineVisibility;
          const visible = shotLineVisible(key);
          button.classList.toggle('line-hidden', !visible);
          button.setAttribute(
            'aria-pressed',
            visible ? 'true' : 'false'
          );
        });
    }

    function saveShotLineVisibility() {
      try {
        localStorage.setItem(
          SHOT_LINE_VISIBILITY_KEY,
          JSON.stringify(shotLineVisibility)
        );
      } catch (_) {}
    }

    function loadShotLineVisibility() {
      let saved = null;
      try {
        saved = JSON.parse(
          localStorage.getItem(
            SHOT_LINE_VISIBILITY_KEY
          ) || 'null'
        );
      } catch (_) {}

      shotLineVisibility = Object.fromEntries(
        Object.keys(SHOT_LINE_VISIBILITY_DEFAULTS).map(
          key => [
            key,
            saved && typeof saved[key] === 'boolean'
              ? saved[key]
              : true
          ]
        )
      );

      syncShotLineVisibilityControls();
    }

    function toggleShotLineVisibility(key) {
      if (
        !Object.prototype.hasOwnProperty.call(
          SHOT_LINE_VISIBILITY_DEFAULTS,
          key
        )
      ) return;

      shotLineVisibility = {
        ...shotLineVisibility,
        [key]:!shotLineVisible(key)
      };

      saveShotLineVisibility();
      syncShotLineLegend();
      redrawShotLineSettings();
    }

    function normalizeShotLineColor(
      key,
      value
    ) {
      const fallback =
        SHOT_LINE_DEFAULTS[key]?.color ||
        '#ffffff';
      const color = String(
        value || ''
      ).trim();

      return /^#[0-9a-f]{6}$/i.test(color)
        ? color.toLowerCase()
        : fallback;
    }

    function normalizeShotLineValue(
      key,
      field,
      value
    ) {
      const fallback =
        SHOT_LINE_DEFAULTS[key]?.[field] ??
        0;
      const number =
        Number(value);

      if (!Number.isFinite(number)) {
        return fallback;
      }

      if (field === 'width') {
        return Math.max(
          .5,
          Math.min(
            5,
            Math.round(number * 10) / 10
          )
        );
      }

      return Math.max(
        0,
        Math.min(
          90,
          Math.round(number / 5) * 5
        )
      );
    }

    function normalizeShotLineSettings(
      source
    ) {
      const input =
        source &&
        typeof source === 'object'
          ? source
          : {};

      return Object.fromEntries(
        Object.keys(
          SHOT_LINE_DEFAULTS
        ).map(key => {
          const candidate =
            input[key] &&
            typeof input[key] === 'object'
              ? input[key]
              : {};

          return [
            key,
            {
              color:
                normalizeShotLineColor(
                  key,
                  candidate.color
                ),
              width:
                normalizeShotLineValue(
                  key,
                  'width',
                  candidate.width
                ),
              transparency:
                normalizeShotLineValue(
                  key,
                  'transparency',
                  candidate.transparency
                )
            }
          ];
        })
      );
    }

    function shotLineStyle(key) {
      const settings =
        shotLineSettings[key] ||
        SHOT_LINE_DEFAULTS[key];

      return {
        color:settings.color,
        width:settings.width,
        opacity:
          Math.max(
            .1,
            Math.min(
              1,
              1 -
                settings.transparency /
                100
            )
          )
      };
    }

    function syncShotLineControls() {
      const names = {
        profile:'Profile',
        pressure:'Pressure',
        flow:'Flow',
        pump:'Pump',
        stages:'Stages'
      };

      Object.entries(names)
        .forEach(([key, suffix]) => {
          const settings =
            shotLineSettings[key];

          const width =
            $('shotLine' +
              suffix +
              'Width');
          const transparency =
            $('shotLine' +
              suffix +
              'Transparency');
          const widthValue =
            $('shotLine' +
              suffix +
              'WidthValue');
          const transparencyValue =
            $('shotLine' +
              suffix +
              'TransparencyValue');

          if (width) width.value = settings.width;
          if (transparency) transparency.value = settings.transparency;
          if (widthValue) {
            widthValue.textContent =
              settings.width.toFixed(1) + ' px';
          }
          if (transparencyValue) {
            transparencyValue.textContent =
              Math.round(settings.transparency) + '%';
          }

          const quickColor =
            $('shotLineQuick' + suffix + 'Color');
          const quickWidth =
            $('shotLineQuick' + suffix + 'Width');
          const quickTransparency =
            $('shotLineQuick' + suffix + 'Transparency');
          const quickWidthValue =
            $('shotLineQuick' + suffix + 'WidthValue');
          const quickTransparencyValue =
            $('shotLineQuick' + suffix + 'TransparencyValue');

          if (quickColor) quickColor.value = settings.color;
          if (quickWidth) quickWidth.value = settings.width;
          if (quickTransparency) {
            quickTransparency.value = settings.transparency;
          }
          if (quickWidthValue) {
            quickWidthValue.textContent =
              settings.width.toFixed(1) + ' px';
          }
          if (quickTransparencyValue) {
            quickTransparencyValue.textContent =
              Math.round(settings.transparency) + '%';
          }
        });

      syncShotLineLegend();
    }

    function syncShotLineLegend() {
      ['profile','pressure','flow','pump']
        .forEach(key => {
          const style = shotLineStyle(key);
          const visible = shotLineVisible(key);

          document
            .querySelectorAll(
              '[data-shot-line-indicator="' + key + '"]'
            )
            .forEach(node => {
              if (key === 'profile') {
                node.style.borderTopColor = style.color;
              } else {
                node.style.backgroundColor = style.color;
              }
              node.style.opacity = String(
                visible ? style.opacity : .28
              );
            });
        });

      syncShotLineVisibilityControls();
    }

    function redrawShotLineSettings() {
      requestAnimationFrame(() => {
        draw();
      });
    }

    function saveShotLineSettings() {
      try {
        localStorage.setItem(
          SHOT_LINE_SETTINGS_KEY,
          JSON.stringify(
            shotLineSettings
          )
        );
      } catch (_) {}
    }

    function updateShotLineSetting(
      key,
      field,
      value
    ) {
      if (
        !Object.prototype.hasOwnProperty.call(
          SHOT_LINE_DEFAULTS,
          key
        ) ||
        ![
          'color',
          'width',
          'transparency'
        ].includes(field)
      ) {
        return;
      }

      shotLineSettings = {
        ...shotLineSettings,
        [key]:{
          ...shotLineSettings[key],
          [field]:
            field === 'color'
              ? normalizeShotLineColor(
                  key,
                  value
                )
              : normalizeShotLineValue(
                  key,
                  field,
                  value
                )
        }
      };

      saveShotLineSettings();
      syncShotLineControls();
      redrawShotLineSettings();
    }

    function cloneShotLineSettings(source = shotLineSettings) {
      return Object.fromEntries(
        Object.entries(source).map(
          ([key, value]) => [
            key,
            {...value}
          ]
        )
      );
    }

    function previewShotLineSetting(
      key,
      field,
      value
    ) {
      if (
        !Object.prototype.hasOwnProperty.call(
          SHOT_LINE_DEFAULTS,
          key
        ) ||
        ![
          'color',
          'width',
          'transparency'
        ].includes(field)
      ) {
        return;
      }

      shotLineSettings = {
        ...shotLineSettings,
        [key]:{
          ...shotLineSettings[key],
          [field]:
            field === 'color'
              ? normalizeShotLineColor(
                  key,
                  value
                )
              : normalizeShotLineValue(
                  key,
                  field,
                  value
                )
        }
      };

      syncShotLineControls();
      redrawShotLineSettings();
    }

    function resetShotLineQuickDraft() {
      shotLineSettings =
        normalizeShotLineSettings(
          SHOT_LINE_DEFAULTS
        );

      syncShotLineControls();
      redrawShotLineSettings();
    }

    function applyShotLineQuickSettings() {
      saveShotLineSettings();
      shotLineQuickBaseline = null;
      toggleShotLineQuickSettings(false);
      toast(
        tr('Настройки линий применены')
      );
    }

    function loadShotLineSettings() {
      let saved = null;

      try {
        saved = JSON.parse(
          localStorage.getItem(
            SHOT_LINE_SETTINGS_KEY
          ) || 'null'
        );
      } catch (_) {}

      shotLineSettings =
        normalizeShotLineSettings(
          saved ||
          SHOT_LINE_DEFAULTS
        );

      syncShotLineControls();
      redrawShotLineSettings();
    }

    function resetShotLineSettings() {
      shotLineSettings =
        normalizeShotLineSettings(
          SHOT_LINE_DEFAULTS
        );

      saveShotLineSettings();
      syncShotLineControls();
      redrawShotLineSettings();
      toast(
        tr(
          'Настройки линий графика сброшены'
        )
      );
    }

    const TYPOGRAPHY_SETTINGS_KEY =
      'silvia-dashboard-typography-v3';

    const TYPOGRAPHY_ZONE_DEFINITIONS = {
      sidebar:{
        selector:'#sidebar',
        label:'Левое меню'
      },
      machine:{
        selector:'.machine-control-card',
        label:'Управление машиной'
      },
      metrics:{
        selector:'.home-metrics-panel',
        label:'Показатели'
      },
      temperature:{
        selector:'#temperatureDetailsCard',
        label:'Температурный график'
      },
      shot:{
        selector:'#shotChartCard',
        label:'График пролива'
      },
      profiles:{
        selector:'#profileLibraryCard, #page-settings, #page-maintenance, #page-diagnostics',
        label:'Профили'
      }
    };

    const TYPOGRAPHY_ZONE_DEFAULTS = {
      sidebar:{
        text:14,
        title:16,
        value:18,
        chart:10
      },
      machine:{
        text:14,
        title:16,
        value:14,
        chart:10
      },
      metrics:{
        text:13,
        title:16,
        value:25,
        chart:10
      },
      temperature:{
        text:12,
        title:20,
        value:16,
        chart:10
      },
      shot:{
        text:12,
        title:16,
        value:16,
        chart:12
      },
      profiles:{
        text:14,
        title:16,
        value:14,
        chart:10
      }
    };

    let typographySettings = {
      zones:Object.fromEntries(
        Object.entries(
          TYPOGRAPHY_ZONE_DEFAULTS
        ).map(([key, value]) => [
          key,
          {...value}
        ])
      )
    };

    let activeChartFontZone = 'shot';

    let homeQuickField = '';
    let homeQuickBaselineCfg = null;
    let homeQuickBaselineDirty = false;
    let homeQuickBaselineUserProfileId = '';
    let homeQuickPhaseSpec = '';
    let homeQuickDirty = false;
    let homeQuickSaving = false;
    let homeQuickPointerId = null;
    let homeQuickChip = null;
    let homeQuickStartX = 0;
    let homeQuickStartY = 0;
    let homeQuickStartValue = 0;
    let homeQuickLastSteps = 0;
    let homeQuickMoved = false;
    let homeQuickKeyboardTimer = 0;

    function clampTypographyPixel(
      value,
      field = 'text'
    ) {
      const ranges = {
        text:[9,22],
        title:[11,32],
        value:[11,52],
        chart:[8,20]
      };
      const range =
        ranges[field] || ranges.text;
      const numeric = Number(value);

      if (!Number.isFinite(numeric)) {
        return range[0];
      }

      return Math.max(
        range[0],
        Math.min(
          range[1],
          Math.round(numeric)
        )
      );
    }

    function typographyZoneSettings(zone) {
      return (
        typographySettings.zones?.[zone] ||
        TYPOGRAPHY_ZONE_DEFAULTS[zone] ||
        {
          text:14,
          title:20,
          value:20,
          chart:10
        }
      );
    }

    function chartFont(size) {
      const zone =
        activeChartFontZone === 'global'
          ? null
          : typographyZoneSettings(
              activeChartFontZone
            );
      const chartBase =
        zone?.chart || 10;
      const pixels = Math.max(
        7,
        Math.round(
          Number(size) *
          chartBase /
          10 *
          10
        ) / 10
      );

      return (
        pixels +
        'px Roboto, Noto, sans-serif'
      );
    }

    function typographyZoneSummary(zone) {
      const settings =
        typographyZoneSettings(zone);

      return [
        settings.text,
        settings.title,
        settings.value,
        settings.chart
      ].join(' / ') + ' px';
    }

    function typographySummaryText() {
      return (
        'Roboto / Noto · ' +
        tr('Независимые размеры в пикселях')
      );
    }

    function typographyExportPayload() {
      return {
        version:3,
        preset:'coffee-dashboard-default-2026-08-05',
        zones:Object.fromEntries(
          Object.entries(
            typographySettings.zones
          ).map(([key, value]) => [
            key,
            {...value}
          ])
        )
      };
    }

    function typographyExportText() {
      return JSON.stringify(
        typographyExportPayload()
      );
    }

    function syncTypographyTransferField(
      force = false
    ) {
      const field =
        $('fontSettingsTransfer');

      if (!field) return;

      const focused =
        document.activeElement === field;

      if (force || !focused) {
        field.value =
          typographyExportText();
      }
    }

    function typographyControlIds(
      zone,
      field
    ) {
      const capZone =
        zone.charAt(0).toUpperCase() +
        zone.slice(1);
      const capField =
        field.charAt(0).toUpperCase() +
        field.slice(1);

      return {
        range:
          'font' +
          capZone +
          capField +
          'Range',
        value:
          'font' +
          capZone +
          capField +
          'Value',
        summary:
          'font' +
          capZone +
          'Summary'
      };
    }

    function syncTypographyControls() {
      Object.keys(
        TYPOGRAPHY_ZONE_DEFAULTS
      ).forEach(zone => {
        const settings =
          typographyZoneSettings(zone);

        ['text','title','value','chart']
          .forEach(field => {
            const ids =
              typographyControlIds(
                zone,
                field
              );
            const range = $(ids.range);
            const output = $(ids.value);

            if (range) {
              range.value =
                String(settings[field]);
            }

            if (output) {
              output.textContent =
                settings[field] + ' px';
            }
          });

        const summaryId =
          typographyControlIds(
            zone,
            'text'
          ).summary;
        const summary =
          $(summaryId);

        if (summary) {
          summary.textContent =
            typographyZoneSummary(zone);
        }
      });

      const summary =
        $('fontSettingsSummary');

      if (summary) {
        summary.textContent =
          typographySummaryText();
      }

      syncTypographyTransferField();
    }

    function setTypographyTokens(
      node,
      settings
    ) {
      if (!node || !settings) return;

      const text =
        clampTypographyPixel(
          settings.text,
          'text'
        );
      const title =
        clampTypographyPixel(
          settings.title,
          'title'
        );
      const value =
        clampTypographyPixel(
          settings.value,
          'value'
        );
      const chart =
        clampTypographyPixel(
          settings.chart,
          'chart'
        );
      const small =
        Math.max(8, text - 2);
      const tiny =
        Math.max(8, text - 4);

      const tokens = {
        '--zone-font-text':
          text + 'px',
        '--zone-font-small':
          small + 'px',
        '--zone-font-tiny':
          tiny + 'px',
        '--zone-font-title':
          title + 'px',
        '--zone-font-value':
          value + 'px',
        '--zone-font-chart':
          chart + 'px',

        '--ha-font-size-xs':
          tiny + 'px',
        '--ha-font-size-s':
          small + 'px',
        '--ha-font-size-m':
          text + 'px',
        '--ha-font-size-l':
          Math.max(
            text + 2,
            Math.min(title, 20)
          ) + 'px',
        '--ha-font-size-xl':
          title + 'px',
        '--ha-font-size-2xl':
          Math.min(
            40,
            title + 4
          ) + 'px',
        '--ha-font-size-3xl':
          value + 'px',
        '--ha-font-size-4xl':
          Math.min(
            56,
            value + 4
          ) + 'px',
        '--ha-font-size-5xl':
          Math.min(
            64,
            value + 12
          ) + 'px'
      };

      Object.entries(tokens).forEach(
        ([name, tokenValue]) => {
          node.style.setProperty(
            name,
            tokenValue
          );
        }
      );
    }

    function applyTypographyZoneVariables() {
      Object.entries(
        TYPOGRAPHY_ZONE_DEFINITIONS
      ).forEach(([key, definition]) => {
        const settings =
          typographyZoneSettings(key);

        document
          .querySelectorAll(
            definition.selector
          )
          .forEach(node => {
            node.dataset.fontZone = key;
            setTypographyTokens(
              node,
              settings
            );
          });
      });
    }

    function normalizeTypographyZone(
      zone,
      source
    ) {
      const defaults =
        TYPOGRAPHY_ZONE_DEFAULTS[zone];

      return {
        text:clampTypographyPixel(
          source?.text ?? defaults.text,
          'text'
        ),
        title:clampTypographyPixel(
          source?.title ?? defaults.title,
          'title'
        ),
        value:clampTypographyPixel(
          source?.value ?? defaults.value,
          'value'
        ),
        chart:clampTypographyPixel(
          source?.chart ?? defaults.chart,
          'chart'
        )
      };
    }

    function migratePercentageTypography(
      source
    ) {
      const global =
        source?.global &&
        typeof source.global === 'object'
          ? source.global
          : source || {};
      const oldZones =
        source?.zones &&
        typeof source.zones === 'object'
          ? source.zones
          : {};

      const globalScale = {
        text:
          (Number(global.ui) || 100) /
          100,
        title:
          (Number(global.title) || 100) /
          100,
        value:
          (Number(global.value) || 100) /
          100,
        chart:
          (Number(global.chart) || 100) /
          100
      };

      return {
        zones:Object.fromEntries(
          Object.entries(
            TYPOGRAPHY_ZONE_DEFAULTS
          ).map(([zone, defaults]) => {
            const oldZone =
              oldZones[zone];
            const zoneScale =
              typeof oldZone === 'number'
                ? oldZone / 100
                : 1;

            return [
              zone,
              {
                text:Math.round(
                  defaults.text *
                  globalScale.text *
                  zoneScale
                ),
                title:Math.round(
                  defaults.title *
                  globalScale.title *
                  zoneScale
                ),
                value:Math.round(
                  defaults.value *
                  globalScale.value *
                  zoneScale
                ),
                chart:Math.round(
                  defaults.chart *
                  globalScale.chart *
                  zoneScale
                )
              }
            ];
          })
        )
      };
    }

    function normalizeTypographySettings(
      settings
    ) {
      let source = settings || {};

      if (
        Number(source.version) < 3 ||
        source.global ||
        typeof source.zones?.sidebar ===
          'number'
      ) {
        source =
          migratePercentageTypography(
            source
          );
      }

      const sourceZones =
        source.zones &&
        typeof source.zones === 'object'
          ? source.zones
          : {};

      return {
        zones:Object.fromEntries(
          Object.keys(
            TYPOGRAPHY_ZONE_DEFAULTS
          ).map(zone => [
            zone,
            normalizeTypographyZone(
              zone,
              sourceZones[zone]
            )
          ])
        )
      };
    }

    function applyTypographySettings(
      settings,
      save = true
    ) {
      typographySettings =
        normalizeTypographySettings(
          settings
        );

      applyTypographyZoneVariables();
      syncTypographyControls();

      if (save) {
        try {
          localStorage.setItem(
            TYPOGRAPHY_SETTINGS_KEY,
            JSON.stringify(
              typographySettings
            )
          );
        } catch (_) {}
      }

      requestAnimationFrame(() => {
        applyTypographyZoneVariables();
        draw();
        drawTemp();
        drawRecipePreview();
      });
    }

    function loadTypographySettings() {
      let saved = null;

      try {
        saved = JSON.parse(
          localStorage.getItem(
            TYPOGRAPHY_SETTINGS_KEY
          ) || 'null'
        );

        if (!saved) {
          saved = JSON.parse(
            localStorage.getItem(
              'silvia-dashboard-typography-v2'
            ) || 'null'
          );
        }

        if (!saved) {
          saved = JSON.parse(
            localStorage.getItem(
              'silvia-dashboard-typography-v1'
            ) || 'null'
          );
        }
      } catch (_) {}

      applyTypographySettings(
        saved || {
          version:3,
          zones:
            TYPOGRAPHY_ZONE_DEFAULTS
        },
        false
      );
    }

    function updateTypographyZoneField(
      zone,
      field,
      value
    ) {
      if (
        !Object.prototype.hasOwnProperty.call(
          TYPOGRAPHY_ZONE_DEFAULTS,
          zone
        ) ||
        !['text','title','value','chart']
          .includes(field)
      ) {
        return;
      }

      const nextZones =
        Object.fromEntries(
          Object.entries(
            typographySettings.zones
          ).map(([key, settings]) => [
            key,
            {...settings}
          ])
        );

      nextZones[zone][field] =
        clampTypographyPixel(
          value,
          field
        );

      applyTypographySettings({
        version:3,
        zones:nextZones
      });
    }

    function resetTypographyZone(zone) {
      if (
        !Object.prototype.hasOwnProperty.call(
          TYPOGRAPHY_ZONE_DEFAULTS,
          zone
        )
      ) {
        return;
      }

      const nextZones =
        Object.fromEntries(
          Object.entries(
            typographySettings.zones
          ).map(([key, settings]) => [
            key,
            {...settings}
          ])
        );

      nextZones[zone] = {
        ...TYPOGRAPHY_ZONE_DEFAULTS[zone]
      };

      applyTypographySettings({
        version:3,
        zones:nextZones
      });

      toast(
        tr('Размеры окна сброшены')
      );
    }

    function resetTypographySettings() {
      applyTypographySettings({
        version:3,
        zones:Object.fromEntries(
          Object.entries(
            TYPOGRAPHY_ZONE_DEFAULTS
          ).map(([key, value]) => [
            key,
            {...value}
          ])
        )
      });

      syncTypographyTransferField(true);
      toast(
        tr('Размеры шрифтов сброшены')
      );
    }

    async function copyTypographySettings() {
      const text =
        typographyExportText();
      const field =
        $('fontSettingsTransfer');

      if (field) {
        field.value = text;
      }

      let copied = false;

      try {
        if (
          navigator.clipboard &&
          window.isSecureContext
        ) {
          await navigator.clipboard.writeText(
            text
          );
          copied = true;
        }
      } catch (_) {}

      if (!copied && field) {
        try {
          field.focus();
          field.select();
          copied =
            document.execCommand('copy');
        } catch (_) {}
      }

      toast(
        tr(
          copied
            ? 'Настройки шрифтов скопированы'
            : 'Скопируйте строку из поля'
        ),
        !copied
      );
    }

    function applyTypographySettingsFromText() {
      const field =
        $('fontSettingsTransfer');

      if (!field) return;

      let parsed = null;

      try {
        parsed = JSON.parse(
          field.value.trim()
        );
      } catch (_) {
        toast(
          tr('Неверный формат настроек'),
          true
        );
        return;
      }

      applyTypographySettings(parsed);
      syncTypographyTransferField(true);
      toast(
        tr('Настройки шрифтов применены')
      );
    }

    function toggleTypographyPanel(force) {
      const panel = $('homeFontTools');
      const button = $('fontSettingsButton');

      if (!panel) return;

      const open =
        typeof force === 'boolean'
          ? force
          : panel.hidden;

      panel.hidden = !open;

      if (button) {
        button.classList.toggle(
          'active',
          open
        );
        button.setAttribute(
          'aria-pressed',
          open ? 'true' : 'false'
        );
      }

      if (open) {
        applyTypographyZoneVariables();
        syncTypographyControls();
        syncShotLineControls();
        syncTypographyTransferField(true);
      }
    }

    function inputNumber(id) {
      const node = $(id);
      const value = node ? Number(node.value) : 0;
      return Number.isFinite(value) ? value : 0;
    }

    function phaseNumber(value, fallback = 0) {
      const number = Number(value);
      return Number.isFinite(number) ? number : fallback;
    }

    function cloneAdvancedPhases(phases) {
      return Array.isArray(phases)
        ? phases.map(item => ({...item}))
        : null;
    }

    function isAdvancedPressurePhase(phase) {
      return !!phase && phase.type !== 'pause';
    }

    function linkAdvancedPressurePhases(phases) {
      if (!Array.isArray(phases)) return phases;
      for (let index=1;index<phases.length;index++) {
        if (
          isAdvancedPressurePhase(phases[index-1]) &&
          isAdvancedPressurePhase(phases[index])
        ) {
          phases[index].start = phaseNumber(phases[index-1].end);
        }
      }
      return phases;
    }

    function normalizeAdvancedPhases(phases) {
      if (!Array.isArray(phases)) return null;
      const clean = phases.slice(0, 12).map(item => {
        if (item?.type === 'pause') {
          return {
            type:'pause',
            duration:Math.max(.5, Math.min(240, phaseNumber(item.duration, 10)))
          };
        }
        return {
          type:item?.type === 'preinfusion' ? 'preinfusion' : 'brew',
          duration:Math.max(.5, Math.min(240, phaseNumber(item?.duration, 10))),
          start:Math.max(0, Math.min(12, phaseNumber(item?.start, .8))),
          end:Math.max(0, Math.min(12, phaseNumber(item?.end, phaseNumber(item?.start, .8)))),
          weight:Math.max(0, Math.min(1, phaseNumber(item?.weight, 0)))
        };
      });
      return clean.length && clean.some(item => item.type !== 'pause') ? clean : null;
    }

    function parseAdvancedPhaseSpec(spec) {
      const value = String(spec || '').trim();
      if (!value) return null;
      const phases = value.split(';').map(record => {
        const fields = record.split(',');
        if (fields[0] === 'P' && fields.length === 2) {
          return {type:'pause',duration:phaseNumber(fields[1],10)};
        }
        if ((fields[0] === 'B' || fields[0] === 'I') && fields.length === 5) {
          return {
            type:fields[0] === 'I' ? 'preinfusion' : 'brew',
            duration:phaseNumber(fields[1],10),
            start:phaseNumber(fields[2],.8),
            end:phaseNumber(fields[3],.8),
            weight:phaseNumber(fields[4],0)
          };
        }
        return null;
      });
      return phases.some(item => !item) ? null : normalizeAdvancedPhases(phases);
    }

    function advancedPhaseSpec(phases = advancedPhaseDraft) {
      const clean = normalizeAdvancedPhases(phases);
      if (!clean) return '';
      return clean.map(item => item.type === 'pause'
        ? ['P', fmt(item.duration, item.duration % 1 ? 1 : 0)].join(',')
        : [
            item.type === 'preinfusion' ? 'I' : 'B',
            fmt(item.duration, item.duration % 1 ? 1 : 0),
            fmt(item.start,1),
            fmt(item.end,1),
            Number(item.weight || 0).toFixed(3)
          ].join(',')
      ).join(';');
    }

    function defaultAdvancedPhases(profileName, config = recipeConfigFromInputs()) {
      const profile = String(profileName || currentProfileName || 'Custom');
      if (profile === 'Funnel') {
        return normalizeAdvancedPhases([
          {type:'preinfusion',duration:12,start:.5,end:.5,weight:0},
          {type:'pause',duration:28},
          {type:'brew',duration:30,start:.8,end:.8,weight:.35},
          {type:'pause',duration:15},
          {type:'brew',duration:30,start:.8,end:.8,weight:.68},
          {type:'pause',duration:15},
          {type:'brew',duration:30,start:.8,end:.8,weight:.92},
          {type:'pause',duration:40}
        ]);
      }

      if (profile === 'Custom' && String(config?.phases || activeCustomPhaseSpec || '').trim()) {
        return parseAdvancedPhaseSpec(config?.phases || activeCustomPhaseSpec);
      }

      const pre = Math.max(0, phaseNumber(config?.prePump));
      const pause = Math.max(0, phaseNumber(config?.prePause));
      const shot = Math.max(.5, phaseNumber(config?.shotSeconds, 30));
      const prePressure = Math.max(0, Math.min(12, phaseNumber(config?.prePower, .8)));
      const mainPressure = Math.max(0, Math.min(12, phaseNumber(config?.mainPressure, 9)));
      const endPressure = Math.max(0, Math.min(12, phaseNumber(config?.endPressure, mainPressure)));
      const phases = [];

      if (pre > .05) {
        phases.push({type:'preinfusion',duration:pre,start:prePressure,end:prePressure,weight:0});
      }
      if (pause > .05) {
        phases.push({type:'pause',duration:pause});
      }

      const ramp = Math.max(
        0,
        Math.min(
          shot,
          phaseNumber(
            builtInProfileRampSeconds(profile, shot),
            phaseNumber(config?.softInfusionTime, 0)
          )
        )
      );

      if (profile !== 'Classic' && ramp > .05 && ramp < shot - .05) {
        phases.push({
          type:'brew',
          duration:ramp,
          start:prePressure,
          end:mainPressure,
          weight:0
        });
        phases.push({
          type:'brew',
          duration:shot-ramp,
          start:mainPressure,
          end:endPressure,
          weight:0
        });
      } else {
        phases.push({
          type:'brew',
          duration:shot,
          start:profile === 'Classic' ? mainPressure : mainPressure,
          end:profile === 'Classic' ? mainPressure : endPressure,
          weight:0
        });
      }

      return normalizeAdvancedPhases(phases);
    }

    function funnelPhaseEditingEnabled() {
      return true;
    }

    function phaseDraftTotalSeconds(phases = advancedPhaseDraft) {
      return (phases || []).reduce(
        (total,item) => total + Math.max(0, phaseNumber(item.duration)),
        0
      );
    }

    function displayedAdvancedPhases(profileName, config = profileDisplayConfig()) {
      if (profilePhaseEditMode && advancedPhaseDraft) {
        return normalizeAdvancedPhases(advancedPhaseDraft);
      }

      const configSpec = String(config?.phases || '').trim();
      if (configSpec) {
        const stored = parseAdvancedPhaseSpec(configSpec);
        if (stored) return stored;
      }

      const profile = String(profileName || currentProfileName || '');
      if (profile === 'Custom' && activeCustomPhaseSpec) {
        const active = parseAdvancedPhaseSpec(activeCustomPhaseSpec);
        if (active) return active;
      }

      return defaultAdvancedPhases(profile, config);
    }

    function advancedPhaseSeries(phases = advancedPhaseDraft) {
      const clean = normalizeAdvancedPhases(phases);
      if (!clean) return [];
      const result = [];
      let elapsed = 0;
      clean.forEach(item => {
        const duration = Math.max(.5, phaseNumber(item.duration));
        const steps = Math.max(2, Math.ceil(duration * 5));
        for (let index = 0; index <= steps; index++) {
          const ratio = index / steps;
          const eased = ratio * ratio * (3 - 2 * ratio);
          result.push({
            x:elapsed + duration * ratio,
            t:item.type === 'pause'
              ? 0
              : phaseNumber(item.start) +
                (phaseNumber(item.end) - phaseNumber(item.start)) * eased
          });
        }
        elapsed += duration;
      });
      return result;
    }

    function phaseDisplayName(phase, index, phases = advancedPhaseDraft) {
      if (phase?.type === 'pause') {
        let number = 0;
        for (let i=0;i<=index;i++) if (phases?.[i]?.type === 'pause') number += 1;
        return tr('Пауза') + ' ' + number;
      }
      if (phase?.type === 'preinfusion') return tr('Предсмачивание');
      let number = 0;
      let total = 0;
      for (let i=0;i<(phases?.length || 0);i++) {
        if (phases[i]?.type === 'brew') {
          total += 1;
          if (i <= index) number += 1;
        }
      }
      return tr('Подача') + (total > 1 ? ' ' + number : '');
    }

    function renderPhaseStripView() {
      const root = $('shotProfileSummary');
      if (!root) return;
      root.innerHTML = '';
      if (chartMode === 'archive') return;

      const phases = displayedAdvancedPhases(
        currentProfileName,
        profileDisplayConfig()
      );
      if (!phases?.length) {
        const empty = document.createElement('div');
        empty.className = 'profile-phase-view-empty';
        empty.textContent = tr('Нет этапов профиля');
        root.appendChild(empty);
        return;
      }

      phases.forEach((phase,index) => {
        const item = document.createElement('div');
        item.className = 'profile-phase-view-item' + (phase.type === 'pause' ? ' pause' : '');
        const small = document.createElement('small');
        small.textContent = phaseDisplayName(phase,index,phases);
        const value = document.createElement('b');
        value.textContent =
          fmt(phase.duration, phase.duration % 1 ? 1 : 0) + ' ' + tr('с') +
          (phase.type !== 'pause'
            ? ' · ' + fmt(phase.start,1) +
              (Math.abs(phase.end-phase.start) > .04 ? '→' + fmt(phase.end,1) : '') +
              ' bar'
            : '');
        item.append(small,value);
        root.appendChild(item);
      });
    }

    function markAdvancedPhasesChanged() {
      advancedPhaseDraft = linkAdvancedPressurePhases(
        normalizeAdvancedPhases(advancedPhaseDraft)
      );
      if (!advancedPhaseDraft) return;

      const totalInput = $('shotSeconds');
      if (totalInput) totalInput.value = Math.min(240, phaseDraftTotalSeconds()).toFixed(1);

      recipeDirty = true;
      mainCurveDirty = true;
      mainCurveEditing = true;
      advancedPhaseEditorOpen = true;

      if (activeUserProfileId) {
        activeUserProfileId = '';
        renderUserProfiles();
      }

      renderAdvancedPhaseEditor();
      updateRecipeProfileUi();
      updateRecipeDescription();
      updateShotScenario();
      fillHomeProfileSelect(availableProfileOptions,'Custom');
      drawRecipePreview();
      requestAnimationFrame(draw);
    }

    function ensureAdvancedPhaseDraft() {
      if (!advancedPhaseDraft) {
        advancedPhaseDraft = cloneAdvancedPhases(
          displayedAdvancedPhases(currentProfileName, profileDisplayConfig())
        );
      }
      advancedPhaseDraft = normalizeAdvancedPhases(advancedPhaseDraft);
      if (!advancedPhaseDraft) return null;

      if (advancedPhaseSelected < 0 || advancedPhaseSelected >= advancedPhaseDraft.length) {
        advancedPhaseSelected = 0;
      }
      return advancedPhaseDraft;
    }

    function beginProfilePhaseEdit() {
      if (!mainCurveCanEdit()) {
        if (shotRunning) toast(tr('Редактирование недоступно во время пролива'), true);
        return;
      }
      if (profilePhaseEditMode) return;

      const phases = linkAdvancedPressurePhases(cloneAdvancedPhases(
        displayedAdvancedPhases(currentProfileName, profileDisplayConfig())
      ));
      if (!phases?.length) return toast(tr('Не удалось подготовить профиль для редактирования'), true);

      const preinfusion = phases.find(item => item.type === 'preinfusion');
      advancedPreinfusionRampMode = !!(
        preinfusion &&
        Math.abs(phaseNumber(preinfusion.end)-phaseNumber(preinfusion.start)) > .04
      );

      profilePhaseBaseline = {
        profile:currentProfileName,
        recipeBaseProfile,
        activeCustomPhaseSpec,
        activeCustomPhaseBase,
        recipeDirty,
        mainCurveDirty
      };
      advancedPhaseDraft = phases;
      advancedPhaseSelected = 0;
      advancedPhaseEditorOpen = true;
      profilePhaseEditMode = true;
      mainCurveEditing = true;
      mainCurveDirty = false;
      chartMode = 'auto';
      hideChartTooltip();
      updateMainCurveEditor();
      renderAdvancedPhaseEditor();
      fillHomeProfileSelect(availableProfileOptions,'Custom');
      requestAnimationFrame(draw);
    }

    function toggleProfilePhaseEdit() {
      if (profilePhaseEditMode) {
        cancelProfilePhaseEdit(false);
        return;
      }
      beginProfilePhaseEdit();
    }

    function cancelProfilePhaseEdit(showMessage = true) {
      if (!profilePhaseEditMode) return;
      const baseline = profilePhaseBaseline;
      advancedPhaseDraft = null;
      advancedPhaseSelected = -1;
      advancedPreinfusionRampMode = false;
      advancedPhaseEditorOpen = false;
      profilePhaseEditMode = false;
      mainCurveEditing = false;
      mainCurveDirty = baseline?.mainCurveDirty || false;
      recipeDirty = baseline?.recipeDirty || false;
      currentProfileName = baseline?.profile || currentProfileName;
      recipeBaseProfile = baseline?.recipeBaseProfile || recipeBaseProfile;
      activeCustomPhaseSpec = baseline?.activeCustomPhaseSpec || activeCustomPhaseSpec;
      activeCustomPhaseBase = baseline?.activeCustomPhaseBase || activeCustomPhaseBase;
      profilePhaseBaseline = null;
      mainCurveDragKey = '';
      mainCurveDragPointerId = null;
      mainCurveDragStartConfig = null;
      syncProfileControls(currentProfileName);
      updateMainCurveEditor();
      updateShotScenario();
      if (showMessage) toast(tr('Изменения профиля отменены'));
    }

    async function saveProfilePhaseEdit() {
      if (!profilePhaseEditMode) return;
      const phases = normalizeAdvancedPhases(advancedPhaseDraft);
      const spec = advancedPhaseSpec(phases);
      if (!spec) return toast(tr('Профиль должен содержать хотя бы одну подачу'), true);

      const total = phaseDraftTotalSeconds(phases);
      if (total > 240.001) {
        return toast(tr('Общее время профиля не должно превышать 240 секунд'), true);
      }
      let previousWeight = 0;
      for (const phase of phases) {
        if (phase.type === 'pause' || !(phase.weight > 0)) continue;
        if (phase.weight <= previousWeight) {
          return toast(tr('Пороги остановки по весу должны возрастать'), true);
        }
        previousWeight = phase.weight;
      }

      const button = $('advancedPhaseDone');
      if (button) button.disabled = true;
      try {
        await applyAdvancedPhaseSpec(spec);
        await post('select','Silvia Shot Profile','set',{option:'Custom'});
        currentProfileName = 'Custom';
        recipeBaseProfile = 'Custom';
        recipeDirty = false;
        mainCurveDirty = false;
        mainCurveEditing = false;
        profilePhaseEditMode = false;
        advancedPhaseEditorOpen = false;
        advancedPhaseDraft = null;
        advancedPhaseSelected = -1;
        advancedPreinfusionRampMode = false;
        profilePhaseBaseline = null;
        syncProfileControls('Custom');
        readProfileCfgFromInputs();
        captureRecipeBaseline('Custom');
        updateMainCurveEditor();
        updateShotScenario();
        toast(tr('Профиль применён как Custom'));
      } catch (error) {
        const reason = String(error?.message || '');
        const detail = reason && !reason.startsWith('custom_phases_')
          ? ': ' + reason.replaceAll('_',' ')
          : '';
        toast(tr('Не удалось применить этапы профиля') + detail, true);
      } finally {
        if (button) button.disabled = false;
      }
    }

    function addAdvancedPressurePoint() {
      if (shotRunning || !profilePhaseEditMode) return;
      const phases = ensureAdvancedPhaseDraft();
      if (!phases) return;
      if (phases.length >= 12) return toast(tr('Достигнут предел этапов'), true);
      const source = phases[advancedPhaseSelected];
      const pressure = source?.type !== 'pause'
        ? phaseNumber(source.end,.8)
        : .8;
      const insertAt = Math.max(0,Math.min(phases.length,advancedPhaseSelected+1));
      phases.splice(insertAt,0,{
        type:'brew',
        duration:20,
        start:pressure,
        end:pressure,
        weight:0
      });
      advancedPhaseSelected = insertAt;
      markAdvancedPhasesChanged();
    }

    function addAdvancedPreinfusion() {
      const phases = ensureAdvancedPhaseDraft();
      if (!phases) return;
      if (phases.some(item => item.type === 'preinfusion')) {
        return toast(tr('Предсмачивание уже добавлено'), true);
      }
      if (phases.length >= 12) return toast(tr('Достигнут предел этапов'), true);
      if (phaseDraftTotalSeconds(phases) + 4 > 240.001) {
        return toast(tr('Общее время профиля не должно превышать 240 секунд'), true);
      }
      const firstBrew = phases.find(item => item.type === 'brew');
      const pressure = firstBrew ? phaseNumber(firstBrew.start,.8) : .8;
      phases.unshift({
        type:'preinfusion',
        duration:4,
        start:pressure,
        end:pressure,
        weight:0
      });
      advancedPreinfusionRampMode = false;
      advancedPhaseSelected = 0;
      markAdvancedPhasesChanged();
    }

    function addAdvancedPause() {
      if (shotRunning || !profilePhaseEditMode) return;
      const phases = ensureAdvancedPhaseDraft();
      if (!phases) return;
      if (phases.length >= 12) return toast(tr('Достигнут предел этапов'), true);
      const insertAt = Math.max(0,Math.min(phases.length,advancedPhaseSelected+1));
      phases.splice(insertAt,0,{type:'pause',duration:10});
      advancedPhaseSelected = insertAt;
      markAdvancedPhasesChanged();
    }

    function removeAdvancedPhase(index) {
      const phases = ensureAdvancedPhaseDraft();
      if (!phases || phases.length <= 1) return;
      const removed = phases.splice(index,1)[0];
      if (removed?.type !== 'pause' && !phases.some(item => item.type !== 'pause')) {
        phases.splice(index,0,removed);
        return toast(tr('Нельзя удалить последнюю подачу'), true);
      }
      advancedPhaseSelected = Math.max(0,Math.min(index,phases.length-1));
      markAdvancedPhasesChanged();
    }

    function duplicateAdvancedPhase(index) {
      const phases = ensureAdvancedPhaseDraft();
      if (!phases || !phases[index]) return;
      if (phases.length >= 12) return toast(tr('Достигнут предел этапов'), true);
      phases.splice(index+1,0,{...phases[index]});
      advancedPhaseSelected = index+1;
      markAdvancedPhasesChanged();
    }

    function moveAdvancedPhase(index,direction) {
      const phases = ensureAdvancedPhaseDraft();
      if (!phases) return;
      const target = index + direction;
      if (target < 0 || target >= phases.length) return;
      [phases[index],phases[target]] = [phases[target],phases[index]];
      advancedPhaseSelected = target;
      markAdvancedPhasesChanged();
    }

    function convertAdvancedPhase(index,type) {
      const phases = ensureAdvancedPhaseDraft();
      const phase = phases?.[index];
      if (!phase || phase.type === type) return;
      if (
        type === 'preinfusion' &&
        phases.some((item,itemIndex) =>
          itemIndex !== index && item.type === 'preinfusion'
        )
      ) {
        return toast(tr('Предсмачивание уже добавлено'), true);
      }
      if (phase.type !== 'pause' && type !== 'pause') {
        phases[index] = {...phase,type:type === 'preinfusion' ? 'preinfusion' : 'brew'};
        if (type === 'preinfusion' && index > 0) {
          const [preinfusion] = phases.splice(index,1);
          phases.unshift(preinfusion);
          advancedPhaseSelected = 0;
        } else {
          advancedPhaseSelected = index;
        }
        markAdvancedPhasesChanged();
        return;
      }
      if (type === 'pause') {
        if (phase.type !== 'pause' && phases.filter(item => item.type !== 'pause').length <= 1) {
          return toast(tr('В профиле должна остаться подача'), true);
        }
        phases[index] = {type:'pause',duration:phase.duration};
      } else {
        phases[index] = {
          type:type === 'preinfusion' ? 'preinfusion' : 'brew',
          duration:phase.duration,
          start:.8,
          end:.8,
          weight:0
        };
      }
      if (type === 'preinfusion' && index > 0) {
        const [preinfusion] = phases.splice(index,1);
        phases.unshift(preinfusion);
        advancedPhaseSelected = 0;
      } else {
        advancedPhaseSelected = index;
      }
      markAdvancedPhasesChanged();
    }

    function setAdvancedPhaseValue(index, field, value) {
      const phases = ensureAdvancedPhaseDraft();
      const phase = phases?.[index];
      if (!phase) return;
      const number = phaseNumber(value);
      if (field === 'duration') phase.duration = Math.max(.5,Math.min(240,number));
      if (phase.type === 'preinfusion' && field === 'pressure') {
        const pressure = Math.max(0,Math.min(12,number));
        phase.start = pressure;
        phase.end = pressure;
        if (isAdvancedPressurePhase(phases[index+1])) {
          phases[index+1].start = pressure;
        }
      }
      if (phase.type !== 'pause' && (field === 'start' || field === 'end')) {
        const pressure = Math.max(0,Math.min(12,number));
        phase[field] = pressure;
        if (field === 'start' && isAdvancedPressurePhase(phases[index-1])) {
          phases[index-1].end = pressure;
        }
        if (field === 'end' && isAdvancedPressurePhase(phases[index+1])) {
          phases[index+1].start = pressure;
        }
      }
      if (phase.type !== 'pause' && field === 'weight') {
        phase.weight = Math.max(0,Math.min(1,number/100));
      }
      advancedPhaseSelected = index;
      markAdvancedPhasesChanged();
    }

    function setAdvancedPreinfusionMode(index,ramp) {
      const phases = ensureAdvancedPhaseDraft();
      const phase = phases?.[index];
      if (!phase || phase.type !== 'preinfusion') return;
      advancedPreinfusionRampMode = !!ramp;
      if (!advancedPreinfusionRampMode) {
        phase.end = phaseNumber(phase.start,.8);
        if (isAdvancedPressurePhase(phases[index+1])) {
          phases[index+1].start = phase.end;
        }
        markAdvancedPhasesChanged();
      } else {
        renderAdvancedPhaseEditor();
        requestAnimationFrame(draw);
      }
    }

    function ensureAdvancedPhaseEditor() {
      const host = $('profilePhaseEditorHost');
      if (!host) return;
      let root = $('advancedPhaseEditor');
      if (root) return;
      root = document.createElement('section');
      root.id = 'advancedPhaseEditor';
      root.className = 'advanced-phase-editor';
      root.innerHTML =
        '<div class="advanced-phase-toolbar">' +
          '<div class="advanced-phase-toolbar-main"><b>' + tr('Редактирование профиля') + '</b>' +
          '<small>' + tr('Точки вверх/вниз — давление, границы влево/вправо — время') + '</small></div>' +
          '<div class="advanced-phase-toolbar-actions">' +
            '<button type="button" onclick="cancelProfilePhaseEdit()">Отмена</button>' +
            '<button id="advancedPhaseDone" class="advanced-phase-done" type="button" onclick="saveProfilePhaseEdit()">✓ Готово</button>' +
          '</div>' +
        '</div>' +
        '<div id="advancedPhaseList" class="advanced-phase-list"></div>' +
        '<div class="advanced-phase-add">' +
          '<button id="advancedPhaseAddPreinfusion" type="button" onclick="addAdvancedPreinfusion()">＋ ' + tr('Предсмачивание') + '</button>' +
          '<button type="button" onclick="addAdvancedPressurePoint()">＋ ' + tr('Подача') + '</button>' +
          '<button type="button" onclick="addAdvancedPause()">＋ ' + tr('Пауза') + '</button>' +
        '</div>' +
        '<div id="advancedPhaseDetail" class="advanced-phase-detail"></div>';
      host.appendChild(root);
    }

    function toggleAdvancedPhaseEditor(force) {
      if (force === false) cancelProfilePhaseEdit(false);
      else if (!profilePhaseEditMode) beginProfilePhaseEdit();
    }

    function renderAdvancedPhaseEditor() {
      ensureAdvancedPhaseEditor();
      const root = $('advancedPhaseEditor');
      const list = $('advancedPhaseList');
      const detail = $('advancedPhaseDetail');
      const card = $('shotChartCard');
      const pencil = $('profilePhaseEditButton');

      if (card) card.classList.toggle('phase-editing',profilePhaseEditMode);
      if (pencil) {
        pencil.classList.toggle('active',profilePhaseEditMode);
        pencil.disabled = shotRunning || chartMode === 'archive';
        pencil.title = tr(profilePhaseEditMode ? 'Редактирование профиля' : 'Редактировать профиль');
        pencil.setAttribute('aria-expanded',profilePhaseEditMode ? 'true' : 'false');
      }

      renderPhaseStripView();

      if (!root || !list || !detail) return;
      root.hidden = !profilePhaseEditMode;
      if (!profilePhaseEditMode) return;

      const phases = ensureAdvancedPhaseDraft();
      if (!phases) return;
      const addPreinfusion = $('advancedPhaseAddPreinfusion');
      if (addPreinfusion) {
        const exists = phases.some(item => item.type === 'preinfusion');
        addPreinfusion.disabled = exists || phases.length >= 12;
        addPreinfusion.title = tr(exists
          ? 'Предсмачивание уже добавлено'
          : 'Добавить предсмачивание в начало профиля');
      }
      list.innerHTML = '';

      phases.forEach((phase,index) => {
        const button = document.createElement('button');
        button.type = 'button';
        button.className =
          'advanced-phase-card' +
          (phase.type === 'pause' ? ' pause' : '') +
          (phase.type === 'preinfusion' ? ' preinfusion' : '') +
          (index === advancedPhaseSelected ? ' selected' : '');
        button.onclick = () => {
          advancedPhaseSelected = index;
          renderAdvancedPhaseEditor();
          requestAnimationFrame(draw);
        };
        button.innerHTML =
          '<span class="advanced-phase-card-index">' + (index+1) + '</span>' +
          '<small>' + phaseDisplayName(phase,index,phases) + '</small>' +
          '<b>' +
            fmt(phase.duration,phase.duration%1?1:0) + ' ' + tr('с') +
            (phase.type !== 'pause'
              ? ' · ' + fmt(phase.start,1) +
                (Math.abs(phase.end-phase.start)>.04?'→'+fmt(phase.end,1):'') +
                ' bar'
              : '') +
          '</b>';
        list.appendChild(button);
      });

      const index = Math.max(0,Math.min(advancedPhaseSelected,phases.length-1));
      advancedPhaseSelected = index;
      const phase = phases[index];
      const field = (label,name,value,min,max,step) =>
        '<label class="advanced-phase-field"><span>' + tr(label) + '</span>' +
        '<input type="number" value="' + value + '" min="' + min + '" max="' + max +
        '" step="' + step + '" onchange="setAdvancedPhaseValue(' + index + ',\'' + name + '\',this.value)"></label>';
      const preinfusionMode = phase.type === 'preinfusion'
        ? '<div class="advanced-phase-type-buttons">' +
            '<button type="button" class="' + (!advancedPreinfusionRampMode?'active':'') +
              '" onclick="setAdvancedPreinfusionMode(' + index + ',false)">' + tr('Постоянное давление') + '</button>' +
            '<button type="button" class="' + (advancedPreinfusionRampMode?'active':'') +
              '" onclick="setAdvancedPreinfusionMode(' + index + ',true)">' + tr('Изменение давления') + '</button>' +
          '</div>'
        : '';
      const pressureFields = phase.type === 'pause'
        ? ''
        : phase.type === 'preinfusion' && !advancedPreinfusionRampMode
          ? field('Давление, bar','pressure',fmt(phase.start,1),0,12,.1) +
            field('Стоп по весу, %','weight',fmt(phase.weight*100,0),0,100,1)
          : field('От, bar','start',fmt(phase.start,1),0,12,.1) +
            field('До, bar','end',fmt(phase.end,1),0,12,.1) +
            field('Стоп по весу, %','weight',fmt(phase.weight*100,0),0,100,1);

      detail.innerHTML =
        '<div class="advanced-phase-detail-head">' +
          '<b>' + phaseDisplayName(phase,index,phases) + '</b>' +
          '<div class="advanced-phase-type-buttons">' +
            '<button type="button" class="' + (phase.type==='preinfusion'?'active':'') +
              '" onclick="convertAdvancedPhase(' + index + ',\'preinfusion\')">' + tr('Предсмачивание') + '</button>' +
            '<button type="button" class="' + (phase.type==='brew'?'active':'') +
              '" onclick="convertAdvancedPhase(' + index + ',\'brew\')">' + tr('Подача') + '</button>' +
            '<button type="button" class="' + (phase.type==='pause'?'active':'') +
              '" onclick="convertAdvancedPhase(' + index + ',\'pause\')">' + tr('Пауза') + '</button>' +
          '</div>' +
        '</div>' +
        preinfusionMode +
        '<div class="advanced-phase-fields">' +
          field('Время, с','duration',fmt(phase.duration,1),.5,240,.5) +
          pressureFields +
        '</div>' +
        '<div class="advanced-phase-detail-actions">' +
          '<button type="button" onclick="moveAdvancedPhase(' + index + ',-1)">← ' + tr('Раньше') + '</button>' +
          '<button type="button" onclick="moveAdvancedPhase(' + index + ',1)">' + tr('Позже') + ' →</button>' +
          '<button type="button" onclick="duplicateAdvancedPhase(' + index + ')">⧉ ' + tr('Дублировать') + '</button>' +
          '<button type="button" class="danger" onclick="removeAdvancedPhase(' + index + ')">' + tr('Удалить') + '</button>' +
        '</div>';
    }

    async function applyAdvancedPhaseSpec(spec) {
      const path = spec
        ? '/custom-phases/apply?' + new URLSearchParams({spec})
        : '/custom-phases/clear';
      const response = await fetch(path,{method:'POST'});
      if (!response.ok) {
        let reason = 'custom_phases_' + response.status;
        try {
          const payload = await response.json();
          if (payload?.error) reason = String(payload.error);
        } catch (_) {}
        throw new Error(reason);
      }
      activeCustomPhaseSpec = String(spec || '');
      activeCustomPhaseBase = spec ? 'Custom' : '';
    }

        function recipeConfigFromInputs() {
      const config = {};
      recipeFieldIds.forEach(id => { config[id] = inputNumber(id); });
      config.phases = profilePhaseEditMode && advancedPhaseDraft
        ? advancedPhaseSpec(advancedPhaseDraft)
        : homeQuickPhaseSpec
          ? String(homeQuickPhaseSpec)
          : (currentProfileName === 'Custom' ? String(activeCustomPhaseSpec || '') : '');
      return config;
    }

    function profileDisplayConfig() {
      const inputs =
        recipeConfigFromInputs();

      if (
        recipeDirty ||
        mainCurveDirty ||
        mainCurveEditing ||
        mainCurveDragKey ||
        homeQuickField
      ) {
        return inputs;
      }

      /*
       * During the first render the hidden editor inputs may still contain
       * their HTML defaults. The visible profile model is already correct,
       * so it must be the source for the graph and summary until editing
       * actually starts.
       */
      return Object.assign(
        inputs,
        profileCfg
      );
    }

    function applyRecipeConfig(config) {
      if (!config) return;
      recipeFieldIds.forEach(id => {
        const node = $(id);
        if (node && Number.isFinite(Number(config[id]))) node.value = config[id];
      });
      advancedPhaseDraft = null;
      advancedPhaseSelected = -1;
      advancedPhaseEditorOpen = false;
      profilePhaseEditMode = false;
      if (String(config.phases || '').trim()) {
        activeCustomPhaseSpec = String(config.phases).trim();
        activeCustomPhaseBase = 'Custom';
      }
      readProfileCfgFromInputs();
      renderAdvancedPhaseEditor();
    }

    function captureRecipeBaseline(profileName) {
      recipeBaselineCfg = recipeConfigFromInputs();
      recipeBaseProfile = profileName || currentProfileName || $('profileSelect')?.value || 'Custom';
      recipeDirty = false;
      updateRecipeProfileUi();
      updateRecipeDescription();
      drawRecipePreview();
    }

    function updateRecipeProfileUi() {
      const select = $('profileSelect');
      if (select) {
        [...select.options].forEach(option => {
          option.textContent = profileLabel(option.value) + (recipeDirty && option.value === 'Custom' ? '*' : '');
        });
        if (recipeDirty && [...select.options].some(option => option.value === 'Custom')) select.value = 'Custom';
      }

      const shown = recipeDirty ? profileLabel('Custom') + '*' : profileLabel(recipeBaseProfile || currentProfileName || 'Custom');
      const badge = $('recipeProfileBadge');
      const status = $('recipePreviewStatus');
      if (badge) { badge.textContent = shown; badge.classList.toggle('dirty', recipeDirty); }
      if (status) {
        status.textContent = tr(recipeApplying ? 'Применение профиля…' : (recipeDirty ? 'Изменения не сохранены' : 'Профиль синхронизирован'));
        status.classList.toggle('dirty', recipeDirty || recipeApplying);
      }
      if ($('recipePreviewProfile')) $('recipePreviewProfile').textContent = shown;
      if ($('recipeEditedLegendText')) $('recipeEditedLegendText').textContent = tr(recipeDirty ? 'Изменённый профиль' : 'Текущий профиль');
      if ($('recipeProfileHint')) {
        $('recipeProfileHint').textContent = recipeApplying
          ? tr('Применение профиля…')
          : recipeDirty
            ? tr('База') + ': ' + profileLabel(recipeBaseProfile || currentProfileName || 'Custom')
            : tr('Профиль синхронизирован');
      }
      if ($('recipeResetButton')) $('recipeResetButton').disabled = !recipeDirty;
      if ($('recipeBaselineLegend')) $('recipeBaselineLegend').hidden = !recipeDirty;
    }

    function recipeScenario(config = recipeConfigFromInputs()) {
      const pre = Math.max(0, Number(config.prePump) || 0);
      const pause = Math.max(0, Number(config.prePause) || 0);
      const shot = Math.max(0, Number(config.shotSeconds) || 0);
      const start = Math.max(0, Number(config.prePower) || 0);
      const main = Math.max(0, Number(config.mainPressure) || 0);
      const end = Math.max(0, Number(config.endPressure) || 0);
      const soft = Math.max(0, Number(config.softInfusionTime) || 0);
      const power = Math.max(0, Number(config.softInfusionStartPower) || 0);
      return [
        tr('Предсмачивание') + ': ' + fmt(pre, pre % 1 ? 1 : 0) + ' ' + tr('с') + ' · ' + fmt(start, 1) + ' ' + tr('бар'),
        tr('Пауза') + ': ' + fmt(pause, pause % 1 ? 1 : 0) + ' ' + tr('с'),
        tr('Основной пролив') + ': ' + fmt(shot, shot % 1 ? 1 : 0) + ' ' + tr('с') + ' · ' + fmt(main, 1) + ' → ' + fmt(end, 1) + ' ' + tr('бар'),
        tr('Мягкий старт') + ': ' + fmt(soft, soft % 1 ? 1 : 0) + ' ' + tr('с') + ' · ' + fmt(power, 0) + '%'
      ].join(' · ');
    }

    function updateRecipeDescription() {
      const config = recipeConfigFromInputs();
      if ($('recipeDescription')) $('recipeDescription').textContent = recipeScenario(config);
      const total = Math.max(0, config.prePump || 0) + Math.max(0, config.prePause || 0) + Math.max(0, config.shotSeconds || 0);
      if ($('recipeTotalTime')) $('recipeTotalTime').textContent = fmt(total, total % 1 ? 1 : 0) + ' ' + tr('с');
      if ($('recipeBrewTemperature')) $('recipeBrewTemperature').textContent = fmt(config.brewTarget, config.brewTarget % 1 ? 1 : 0) + ' °C';
      if ($('recipePhaseStrip')) {
        $('recipePhaseStrip').innerHTML =
          '<span><small>' + tr('Предсмачивание') + '</small><b>' + fmt(config.prePump, config.prePump % 1 ? 1 : 0) + ' ' + tr('с') + ' · ' + fmt(config.prePower, 1) + ' ' + tr('бар') + '</b></span>' +
          '<span><small>' + tr('Пауза') + '</small><b>' + fmt(config.prePause, config.prePause % 1 ? 1 : 0) + ' ' + tr('с') + '</b></span>' +
          '<span><small>' + tr('Основной пролив') + '</small><b>' + fmt(config.shotSeconds, config.shotSeconds % 1 ? 1 : 0) + ' ' + tr('с') + ' · ' + fmt(config.mainPressure, 1) + ' → ' + fmt(config.endPressure, 1) + ' ' + tr('бар') + '</b></span>';
      }
      if ($('recipeSoftStartSummary')) {
        $('recipeSoftStartSummary').textContent = tr('Мягкий старт') + ': ' + fmt(config.softInfusionTime, config.softInfusionTime % 1 ? 1 : 0) + ' ' + tr('с') + ' · ' + tr('Старт помпы') + ': ' + fmt(config.softInfusionStartPower, 0) + '%';
      }
    }

    function recipeFieldChanged() {
      if (!recipeBaselineCfg) {
        recipeBaselineCfg = recipeConfigFromInputs();
        recipeBaseProfile =
          currentProfileName ||
          $('profileSelect')?.value ||
          'Custom';
      }

      readProfileCfgFromInputs();
      recipeDirty = true;

      if (activeUserProfileId) {
        activeUserProfileId = '';
        renderUserProfiles();
      }

      updateRecipeProfileUi();
      updateRecipeDescription();
      updateChartControls();
      drawRecipePreview();
    }

    function stepRecipeValue(id, delta) {
      const input = $(id);
      if (!input) return;
      const step = Number(input.step) || Math.abs(Number(delta)) || 1;
      const min = input.min === '' ? -Infinity : Number(input.min);
      const max = input.max === '' ? Infinity : Number(input.max);
      const decimals = (String(step).split('.')[1] || '').length;
      const next = Math.max(min, Math.min(max, (Number(input.value) || 0) + Number(delta)));
      input.value = decimals ? next.toFixed(decimals) : String(Math.round(next));
      recipeFieldChanged();
    }


    function homeScrubChip(field) {
      return document.querySelector('[data-home-scrub="' + field + '"]');
    }

    function homeScrubDecimals(step) {
      return (String(step).split('.')[1] || '').length;
    }

    function homeScrubValueText(field, value) {
      const meta = homeQuickFieldMeta[field];
      const number = Number(value);
      if (!meta || !Number.isFinite(number)) return '—';
      const decimals = homeScrubDecimals(meta.step);
      const formatted = decimals ? number.toFixed(decimals) : String(Math.round(number));
      return formatted + (meta.unit ? ' ' + tr(meta.unit) : '');
    }

    function normalizeHomeScrubValue(field, value) {
      const meta = homeQuickFieldMeta[field];
      if (!meta) return Number(value) || 0;
      const step = Math.abs(Number(meta.step)) || 1;
      const min = Number.isFinite(Number(meta.min)) ? Number(meta.min) : -Infinity;
      const max = Number.isFinite(Number(meta.max)) ? Number(meta.max) : Infinity;
      const decimals = homeScrubDecimals(step);
      const clamped = Math.max(min, Math.min(max, Number(value) || 0));
      const rounded = Math.round(clamped / step) * step;
      return Number(decimals ? rounded.toFixed(decimals) : Math.round(rounded));
    }

    function syncHomeProfileEditor() {
      /*
       * Значение уже отображается непосредственно в карточке.
       * Отдельное всплывающее значение не используется.
       */
    }

    function clearHomeScrubTimer() {
      if (homeQuickKeyboardTimer) clearTimeout(homeQuickKeyboardTimer);
      homeQuickKeyboardTimer = 0;
    }

    function releaseHomeScrubPointer() {
      if (homeQuickChip && homeQuickPointerId !== null && homeQuickChip.hasPointerCapture?.(homeQuickPointerId)) {
        try { homeQuickChip.releasePointerCapture(homeQuickPointerId); } catch (_) {}
      }
      homeQuickPointerId = null;
    }

    function clearHomeScrubState() {
      clearHomeScrubTimer();
      releaseHomeScrubPointer();
      if (homeQuickChip) {
        homeQuickChip.classList.remove('scrubbing','saving');
        delete homeQuickChip.dataset.scrubValue;
      }
      document.body.classList.remove('home-value-scrubbing');
      homeQuickField = '';
      homeQuickBaselineCfg = null;
      homeQuickBaselineDirty = false;
      homeQuickBaselineUserProfileId = '';
      homeQuickPhaseSpec = '';
      homeQuickDirty = false;
      homeQuickSaving = false;
      homeQuickChip = null;
      homeQuickStartX = 0;
      homeQuickStartY = 0;
      homeQuickStartValue = 0;
      homeQuickLastSteps = 0;
      homeQuickMoved = false;
      updateQuickActions();
    }

    function beginHomeValueSession(field, chip) {
      if (homeQuickField || homeQuickSaving) return false;

      const source = $(field);
      const meta = homeQuickFieldMeta[field];
      const current = Number(source?.value);

      if (!source || !meta || !Number.isFinite(current)) return false;

      if (mainCurveEditing) {
        /*
         * Switching from curve dragging to the quick duration editor must not
         * call cancelMainCurveEditor(): cancel restores the built-in profile
         * snapshot (for Classic that is 9 bar) and discards the pressure draft.
         * End only the active interaction; keep the edited values and dirty
         * state so the duration change continues from the visible curve.
         */
        mainCurveEditing = false;
        mainCurveDragKey = '';
        mainCurveHoverKey = '';
        mainCurveDragPointerId = null;
        mainCurveDragStartConfig = null;
        updateMainCurveEditor();
      }

      /*
       * Перед переходом к Custom синхронизируем скрытые поля с той
       * кривой, которую пользователь действительно видит на графике.
       */
      prepareProfileForCurveDraft();

      homeQuickField = field;
      homeQuickChip = chip || homeScrubChip(field);
      homeQuickBaselineCfg = recipeConfigFromInputs();
      homeQuickBaselineDirty = recipeDirty;
      homeQuickBaselineUserProfileId = activeUserProfileId;
      homeQuickPhaseSpec = (field === 'brewTarget' || field === 'targetWeight')
        ? advancedPhaseSpec(
            displayedAdvancedPhases(
              currentProfileName,
              profileDisplayConfig()
            )
          )
        : '';
      homeQuickDirty = false;
      homeQuickSaving = false;
      homeQuickStartValue = normalizeHomeScrubValue(field, current);
      homeQuickLastSteps = 0;
      homeQuickMoved = false;

      homeQuickChip?.classList.add('scrubbing');
      document.body.classList.add('home-value-scrubbing');
      updateQuickActions();

      return true;
    }

    function beginHomeValueScrub(event) {
      if (event.button > 0 || shotRunning || recipeApplying || homeProfileApplying || mainCurveDragKey) return;
      const chip = event.currentTarget;
      const field = chip?.dataset.homeScrub;
      if (!field || !beginHomeValueSession(field, chip)) return;
      homeQuickPointerId = event.pointerId;
      homeQuickStartX = event.clientX;
      homeQuickStartY = event.clientY;
      try { chip.setPointerCapture(event.pointerId); } catch (_) {}
      event.preventDefault();
      event.stopPropagation();
    }

    function setHomeScrubValue(value) {
      if (!homeQuickField) return false;
      const source = $(homeQuickField);
      const next = normalizeHomeScrubValue(homeQuickField, value);
      const current = Number(source?.value);
      if (!source || !Number.isFinite(next) || (Number.isFinite(current) && Math.abs(current-next) < 0.000001)) return false;
      const step = Math.abs(Number(homeQuickFieldMeta[homeQuickField]?.step)) || 1;
      const decimals = homeScrubDecimals(step);
      source.value = decimals ? next.toFixed(decimals) : String(Math.round(next));
      recipeFieldChanged();
      homeQuickDirty = true;
      updateShotScenario();
      syncHomeProfileEditor();
      draw();
      return true;
    }

    function moveHomeValueScrub(event) {
      if (!homeQuickField || homeQuickPointerId === null || event.pointerId !== homeQuickPointerId || homeQuickSaving) return;
      const dx = event.clientX - homeQuickStartX;
      const dy = event.clientY - homeQuickStartY;
      if (!homeQuickMoved) {
        if (Math.abs(dy) > 10 && Math.abs(dy) > Math.abs(dx) * 1.2) {
          cancelHomeQuickEditor(false);
          return;
        }
        if (Math.abs(dx) < 5) return;
        homeQuickMoved = true;
      }
      const meta = homeQuickFieldMeta[homeQuickField];
      const pixels = Math.max(3, Number(meta?.pixelsPerStep) || 8);
      const steps = Math.round(dx / pixels);
      if (steps === homeQuickLastSteps) {
        event.preventDefault();
        return;
      }
      homeQuickLastSteps = steps;
      setHomeScrubValue(homeQuickStartValue + steps * (Math.abs(Number(meta?.step)) || 1));
      event.preventDefault();
    }

    async function finishHomeValueScrub(event) {
      if (
        !homeQuickField ||
        (
          event &&
          homeQuickPointerId !== null &&
          event.pointerId !== homeQuickPointerId
        ) ||
        homeQuickSaving
      ) {
        return;
      }

      releaseHomeScrubPointer();

      if (!homeQuickMoved || !homeQuickDirty) {
        clearHomeScrubState();
        return;
      }

      homeQuickSaving = true;
      homeQuickChip?.classList.remove('scrubbing');
      homeQuickChip?.classList.add('saving');
      document.body.classList.remove('home-value-scrubbing');

      const editedConfig = recipeConfigFromInputs();
      if (homeQuickPhaseSpec) editedConfig.phases = homeQuickPhaseSpec;
      const baseline = homeQuickBaselineCfg;
      const baselineDirty = homeQuickBaselineDirty;
      const baselineProfileId = homeQuickBaselineUserProfileId;

      try {
        /*
         * Сохраняем полный снимок Custom, а не только изменённую цифру:
         * так не теряются точки текущего встроенного профиля.
         */
        for (const [fieldId, entityName] of Object.entries(nums)) {
          const value = Number(editedConfig[fieldId]);
          if (Number.isFinite(value)) {
            await post('number', entityName, 'set', { value });
          }
        }

        if (homeQuickPhaseSpec) {
          await applyAdvancedPhaseSpec(homeQuickPhaseSpec);
        }

        await post(
          'select',
          'Silvia Shot Profile',
          'set',
          { option:'Custom' }
        );

        /*
         * Native ESPHome-style flow: do not poll the Select back. The edited
         * values are already the exact local snapshot; ESP confirmations and
         * any corrections arrive through SSE.
         */
        applyRecipeConfig(editedConfig);
        readProfileCfgFromInputs();

        currentProfileName = 'Custom';
        recipeBaseProfile = 'Custom';
        recipeDirty = false;
        chartMode = 'auto';
        activeUserProfileId = '';

        syncProfileControls('Custom');
        clearMainCurveDraftState();
        captureRecipeBaseline('Custom');
        renderUserProfiles();
        updateRecipeProfileUi();
        updateRecipeDescription();
        updateMainCurveEditor();
        updateShotScenario();
        draw();
        drawRecipePreview();
      } catch (_) {
        if (baseline) {
          applyRecipeConfig(baseline);
          recipeDirty = baselineDirty;
          activeUserProfileId = baselineProfileId;
          renderUserProfiles();
          updateRecipeProfileUi();
          updateRecipeDescription();
          drawRecipePreview();
          updateShotScenario();
          draw();
        }

        toast(tr('Ошибка сохранения'), true);
      } finally {
        clearHomeScrubState();
        updateMainCurveEditor();
        draw();
      }
    }

    function cancelHomeQuickEditor(showMessage = false) {
      const baseline = homeQuickBaselineCfg;
      const baselineDirty = homeQuickBaselineDirty;
      const baselineProfileId = homeQuickBaselineUserProfileId;
      if (baseline) {
        applyRecipeConfig(baseline);
        recipeDirty = baselineDirty;
        activeUserProfileId = baselineProfileId;
        renderUserProfiles();
        updateRecipeProfileUi();
        updateRecipeDescription();
        drawRecipePreview();
        updateShotScenario();
        draw();
      }
      clearHomeScrubState();
      if (showMessage) toast(tr('Изменение отменено'));
    }

    function scheduleHomeValueKeyboardSave() {
      clearHomeScrubTimer();
      homeQuickKeyboardTimer = setTimeout(() => finishHomeValueScrub(), 450);
    }

    function handleHomeValueScrubKey(event) {
      const field = event.currentTarget?.dataset.homeScrub;
      if (!field || shotRunning || recipeApplying || homeProfileApplying || mainCurveDragKey) return;
      if (event.key === 'Escape') {
        if (homeQuickField === field) {
          event.preventDefault();
          cancelHomeQuickEditor(true);
        }
        return;
      }
      if (event.key === 'Enter') {
        if (homeQuickField === field && homeQuickDirty) {
          event.preventDefault();
          clearHomeScrubTimer();
          finishHomeValueScrub();
        }
        return;
      }
      if (event.key !== 'ArrowLeft' && event.key !== 'ArrowRight') return;
      event.preventDefault();
      if (!homeQuickField && !beginHomeValueSession(field, event.currentTarget)) return;
      if (homeQuickField !== field) return;
      const meta = homeQuickFieldMeta[field];
      const direction = event.key === 'ArrowRight' ? 1 : -1;
      homeQuickMoved = true;
      setHomeScrubValue(Number($(field)?.value) + direction * (Math.abs(Number(meta?.step)) || 1));
      scheduleHomeValueKeyboardSave();
    }

    function initHomeValueScrub() {
      document.querySelectorAll('[data-home-scrub]').forEach(chip => {
        chip.addEventListener('pointerdown', beginHomeValueScrub);
        chip.addEventListener('keydown', handleHomeValueScrubKey);
      });
      document.addEventListener('pointermove', moveHomeValueScrub, {passive:false});
      document.addEventListener('pointerup', finishHomeValueScrub);
      document.addEventListener('pointercancel', event => {
        if (homeQuickPointerId === event.pointerId) cancelHomeQuickEditor(false);
      });
    }

    function resetRecipeChanges() {
      if (!recipeDirty || !recipeBaselineCfg) {
        toast(tr('Нет изменений для сброса'));
        return;
      }
      applyRecipeConfig(recipeBaselineCfg);
      recipeDirty = false;
      const select = $('profileSelect');
      if (select && [...select.options].some(option => option.value === recipeBaseProfile)) select.value = recipeBaseProfile;
      updateRecipeProfileUi();
      updateRecipeDescription();
      drawRecipePreview();
      toast(tr('Изменения сброшены'));
    }


    let values = { pressure: 0, target: 0, flow: 0, pump: 0, temp: 0, tempTarget: 0, heat: 0, autoOffRemaining: NaN, monthlyShots: NaN, monthlyDrinkWeight: NaN };
    let profileCfg = {
      prePump:0,
      prePause:0,
      prePower:0,
      mainPressure:0,
      endPressure:0,
      shotSeconds:0,
      softInfusionTime:0,
      softInfusionStartPower:0
    };
    let currentProfileName = '';
    let liveSeries = [];
    let lastShotSeries = [];
    let archiveSeries = [];
    let tempSeries = [];
    let chartMode = 'auto';
    let archiveTitle = '';

    let archiveIndex = [];
    let archiveSelectedId = 0;
    let archiveSelectedSummary = null;
    let latestShotSummary = null;
    let shotCompletedThisSession = false;
    let archiveDetailSeries = [];
    let archiveListLoadToken = 0;
    const archiveSummaryCache = new Map();
    const archiveSeriesCache = new Map();
    let trainingDataset = null;
    let trainingDatasetBusy = false;
    let trainingExportBusy = false;

    const HOME_SHOT_RAIL_MODE_KEY = 'silvia-home-shot-rail-mode-v1';
    let homeShotRailMode = 'profiles';
    let homeArchiveLoadToken = 0;

    const SETTINGS_PANEL_STORAGE_KEY = 'silvia-settings-panel-v2';
    let settingsPanelCurrent = 'machine';

    const MAINTENANCE_PANEL_STORAGE_KEY = 'silvia-maintenance-panel-v1';
    const DIAGNOSTICS_PANEL_STORAGE_KEY = 'silvia-diagnostics-panel-v1';
    let maintenancePanelCurrent = 'scale';
    let diagnosticsPanelCurrent = 'pressure';
    let lastPoint = 0;
    let lastTempPoint = 0;
    let tempWindowSeconds = 600;
    let homeTemperatureMode = 'state';
    const maxTempHistorySeconds = 600;
    let liveStart = 0;
    let shotRunning = false;
    let shotPhaseConfig = null;
    let shotPeakWeight = NaN;
    let shotTargetWeightAtStart = NaN;
    let lastShotDrinkWeight = NaN;
    let lastShotTargetWeight = NaN;
    let postShotWeightCaptureUntil = 0;
    let machineOn = false;
    let powerCommandPending = false;
    let powerDesiredState = null;
    let powerCommandStartedAt = 0;
    let powerConfirmTimer = 0;
    let hotWaterOn = false;
    let steamModeOn = false;
    let waterAvailable = null;
    let chartHoverIndex = -1;

    function fmt(value, digits = 1) {
      const number = Number(value);
      return Number.isFinite(number) ? number.toFixed(digits) : '—';
    }

    function toast(message, bad = false) {
      const node = $('toast');
      node.textContent = message;
      node.style.color = bad ? '#ffaaaa' : '#bdf4d2';
      node.classList.add('show');
      setTimeout(() => node.classList.remove('show'), 2200);
    }

    function toggleSidebar() {
      if (matchMedia('(max-width:840px)').matches) {
        document.body.classList.toggle('nav-open');
      } else {
        document.body.classList.toggle('sidebar-collapsed');
        try { localStorage.setItem('silvia-sidebar-collapsed', document.body.classList.contains('sidebar-collapsed') ? '1' : '0'); } catch (_) {}
        requestAnimationFrame(() => { draw(); drawTemp(); });
      }
    }

    function closeSidebar() {
      document.body.classList.remove('nav-open');
    }

    const HOME_DASHBOARD_LAYOUT_KEY = 'silvia-home-dashboard-layout-v2';
    const HOME_DASHBOARD_VISIBILITY_KEY = 'silvia-home-dashboard-hidden-v1';
    const SHOT_PROFILE_PANEL_ORDER_KEY =
      'silvia-shot-profile-panel-order-v1';
    const SHOT_PROFILE_PANEL_SIZE_KEY =
      'silvia-shot-profile-panel-size-v1';
    const SHOT_PROFILE_DEFAULT_SHARE = 36;
    const HOME_DASHBOARD_DEFAULT_ORDER = ['overview', 'shot'];
    const HOME_DASHBOARD_TITLES = {
      overview:'Состояние машины',
      shot:'График пролива'
    };
    let homeDashboardEditing = false;
    let homeDashboardDragBlock = null;
    let homeDashboardDragHandle = null;
    let homeDashboardPointerId = null;
    let homeDashboardPressTimer = 0;
    let homeDashboardPressStart = null;
    let homeDashboardHidden = new Set();
    let shotPanelDragPanel = null;
    let shotPanelDragHandle = null;
    let shotPanelPointerId = null;
    let shotPanelPressStart = null;
    let shotPanelMoved = false;
    let shotPanelSwapped = false;
    let shotProfileShare =
      SHOT_PROFILE_DEFAULT_SHARE;
    let shotProfileResizeHandle = null;
    let shotProfileResizePointerId = null;

    const HOME_TOP_PANEL_ORDER_KEY =
      'silvia-home-top-panel-order-v5';
    const HOME_TOP_PANEL_SIZE_KEY =
      'silvia-home-top-panel-size-v5';
    const HOME_TOP_PANEL_DEFAULT_ORDER = [
      'machine',
      'metrics'
    ];
    const HOME_INFO_CARD_ORDER_KEY = 'silvia-home-info-card-order-v1';
    const HOME_INFO_CARD_DEFAULT_ORDER = ['pressure', 'stats', 'temperature'];
    let homeInfoCardDrag = null;
    let homeInfoCardHandle = null;
    let homeInfoCardPointerId = null;
    const HOME_TOP_MACHINE_SHARE_DEFAULT = 56;
    let homeTopPanelDragPanel = null;
    let homeTopPanelDragHandle = null;
    let homeTopPanelPointerId = null;
    let homeTopPanelPressStart = null;
    let homeTopPanelMoved = false;
    let homeTopPanelSwapped = false;
    let homeTopMachineShare =
      HOME_TOP_MACHINE_SHARE_DEFAULT;
    let homeTopResizeHandle = null;
    let homeTopResizePointerId = null;

    const MACHINE_CONTROL_SIZE_KEY =
      'silvia-machine-control-size-v2';
    const HOME_SECTION_GAP_KEY =
      'silvia-home-section-gap-v1';
    const HOME_SECTION_GAP_DEFAULT = 18;
    let homeSectionGap =
      HOME_SECTION_GAP_DEFAULT;

    const HOME_TEMPERATURE_CARD_HEIGHT_KEY =
      'silvia-home-temperature-card-height-v1';
    const HOME_TEMPERATURE_CARD_HEIGHT_DEFAULT = 165;
    let homeTemperatureCardHeight =
      HOME_TEMPERATURE_CARD_HEIGHT_DEFAULT;

    const HOME_TOP_COLUMN_SPLIT_KEY =
      'silvia-home-top-column-split-v2';
    const HOME_TOP_COLUMN_SPLIT_DEFAULT = 38;
    let homeTopColumnSplit =
      HOME_TOP_COLUMN_SPLIT_DEFAULT;
    const MACHINE_CONTROL_SIZE_DEFAULTS = {
      actionIcon:48,
      powerButton:52
    };
    let machineControlSizing = {
      ...MACHINE_CONTROL_SIZE_DEFAULTS
    };








    function clampHomeTopColumnSplit(value) {
      const numeric = Number(value);

      if (!Number.isFinite(numeric)) {
        return HOME_TOP_COLUMN_SPLIT_DEFAULT;
      }

      return Math.max(
        34,
        Math.min(
          48,
          Math.round(numeric)
        )
      );
    }

    function syncHomeTopColumnSplitControl() {
      const input = $('homeTopColumnSplit');
      const output = $('homeTopColumnSplitValue');
      const pressureShare = 18;
      const statsShare = 18;
      const rightShare =
        100 -
        pressureShare -
        statsShare -
        homeTopColumnSplit;

      if (input) {
        input.value = String(homeTopColumnSplit);
      }

      if (output) {
        output.textContent =
          homeTopColumnSplit +
          ' / ' +
          pressureShare +
          ' / ' +
          statsShare +
          ' / ' +
          rightShare;
      }
    }

    function applyHomeTopColumnSplit(
      value,
      persist = true
    ) {
      homeTopColumnSplit =
        clampHomeTopColumnSplit(value);

      const pressureShare = 18;
      const statsShare = 18;
      const rightShare =
        100 -
        pressureShare -
        statsShare -
        homeTopColumnSplit;
      const page = $('page-home');

      if (page) {
        page.style.setProperty(
          '--home-left-column-share',
          homeTopColumnSplit + 'fr'
        );
        page.style.setProperty(
          '--home-pressure-column-share',
          pressureShare + 'fr'
        );
        page.style.setProperty(
          '--home-stats-column-share',
          statsShare + 'fr'
        );
        page.style.setProperty(
          '--home-right-column-share',
          rightShare + 'fr'
        );
      }

      syncHomeTopColumnSplitControl();

      requestAnimationFrame(() => {
        drawHomePressureGauge();
        drawTemp();
      });

      if (persist) {
        try {
          localStorage.setItem(
            HOME_TOP_COLUMN_SPLIT_KEY,
            String(homeTopColumnSplit)
          );
        } catch (_) {}
      }
    }

    function loadHomeTopColumnSplit() {
      let value =
        HOME_TOP_COLUMN_SPLIT_DEFAULT;

      try {
        const raw = localStorage.getItem(
          HOME_TOP_COLUMN_SPLIT_KEY
        );

        if (
          raw !== null &&
          String(raw).trim() !== ''
        ) {
          const saved = Number(raw);

          if (Number.isFinite(saved)) {
            value = saved;
          }
        }
      } catch (_) {}

      applyHomeTopColumnSplit(
        value,
        false
      );
    }

    function setHomeTopColumnSplit(value) {
      applyHomeTopColumnSplit(value);
    }

    function resetHomeTopColumnSplit() {
      applyHomeTopColumnSplit(
        HOME_TOP_COLUMN_SPLIT_DEFAULT
      );
    }

    function clampHomeTemperatureCardHeight(value) {
      const numeric = Number(value);

      if (!Number.isFinite(numeric)) {
        return HOME_TEMPERATURE_CARD_HEIGHT_DEFAULT;
      }

      return Math.max(
        145,
        Math.min(
          260,
          Math.round(numeric)
        )
      );
    }

    function syncHomeTemperatureCardHeightControl() {
      const input = $('homeTemperatureCardHeight');
      const output = $('homeTemperatureCardHeightValue');

      if (input) {
        input.value = String(
          homeTemperatureCardHeight
        );
      }

      if (output) {
        output.textContent =
          homeTemperatureCardHeight + ' px';
      }
    }

    function applyHomeTemperatureCardHeight(
      value,
      persist = true
    ) {
      homeTemperatureCardHeight =
        clampHomeTemperatureCardHeight(value);

      const page = $('page-home');

      if (page) {
        page.style.setProperty(
          '--home-temperature-card-height',
          homeTemperatureCardHeight + 'px'
        );
      }

      syncHomeTemperatureCardHeightControl();

      requestAnimationFrame(() => {
        drawHomePressureGauge();
        drawHomeTemperatureMini();
        drawTemp();
      });

      if (persist) {
        try {
          localStorage.setItem(
            HOME_TEMPERATURE_CARD_HEIGHT_KEY,
            String(homeTemperatureCardHeight)
          );
        } catch (_) {}
      }
    }

    function loadHomeTemperatureCardHeight() {
      let value =
        HOME_TEMPERATURE_CARD_HEIGHT_DEFAULT;

      try {
        const raw = localStorage.getItem(
          HOME_TEMPERATURE_CARD_HEIGHT_KEY
        );

        if (
          raw !== null &&
          String(raw).trim() !== ''
        ) {
          const saved = Number(raw);

          if (Number.isFinite(saved)) {
            value = saved;
          }
        }
      } catch (_) {}

      applyHomeTemperatureCardHeight(
        value,
        false
      );
    }

    function setHomeTemperatureCardHeight(value) {
      applyHomeTemperatureCardHeight(value);
    }

    function resetHomeTemperatureCardHeight() {
      applyHomeTemperatureCardHeight(
        HOME_TEMPERATURE_CARD_HEIGHT_DEFAULT
      );
    }

    function clampHomeSectionGap(value) {
      const numeric = Number(value);

      if (!Number.isFinite(numeric)) {
        return HOME_SECTION_GAP_DEFAULT;
      }

      return Math.max(
        8,
        Math.min(
          40,
          Math.round(numeric)
        )
      );
    }

    function syncHomeSectionGapControl() {
      const input = $('homeSectionGap');
      const output = $('homeSectionGapValue');

      if (input) {
        input.value = String(homeSectionGap);
      }

      if (output) {
        output.textContent =
          homeSectionGap + ' px';
      }
    }

    function applyHomeSectionGap(
      value,
      persist = true
    ) {
      homeSectionGap =
        clampHomeSectionGap(value);

      const page = $('page-home');

      if (page) {
        page.style.setProperty(
          '--home-section-gap',
          homeSectionGap + 'px'
        );
      }

      syncHomeSectionGapControl();

      if (persist) {
        try {
          localStorage.setItem(
            HOME_SECTION_GAP_KEY,
            String(homeSectionGap)
          );
        } catch (_) {}
      }
    }

    function loadHomeSectionGap() {
      let value =
        HOME_SECTION_GAP_DEFAULT;

      try {
        const raw = localStorage.getItem(
          HOME_SECTION_GAP_KEY
        );

        if (
          raw !== null &&
          String(raw).trim() !== ''
        ) {
          const saved = Number(raw);

          if (Number.isFinite(saved)) {
            value = saved;
          }
        }
      } catch (_) {}

      applyHomeSectionGap(
        value,
        false
      );
    }

    function setHomeSectionGap(value) {
      applyHomeSectionGap(value);
    }

    function resetHomeSectionGap() {
      applyHomeSectionGap(
        HOME_SECTION_GAP_DEFAULT
      );
    }

    function clampMachineControlSize(
      value,
      minimum,
      maximum,
      fallback
    ) {
      const numeric = Number(value);

      if (!Number.isFinite(numeric)) {
        return fallback;
      }

      return Math.max(
        minimum,
        Math.min(
          maximum,
          Math.round(numeric)
        )
      );
    }

    function syncMachineControlSizeInputs() {
      const actionInput = $(
        'machineActionIconSize'
      );
      const actionOutput = $(
        'machineActionIconSizeValue'
      );
      const powerInput = $(
        'machinePowerButtonSize'
      );
      const powerOutput = $(
        'machinePowerButtonSizeValue'
      );

      if (actionInput) {
        actionInput.value =
          String(
            machineControlSizing.actionIcon
          );
      }

      if (actionOutput) {
        actionOutput.textContent =
          machineControlSizing.actionIcon +
          ' px';
      }

      if (powerInput) {
        powerInput.value =
          String(
            machineControlSizing.powerButton
          );
      }

      if (powerOutput) {
        powerOutput.textContent =
          machineControlSizing.powerButton +
          ' px';
      }
    }

    function storeMachineControlSizing() {
      try {
        localStorage.setItem(
          MACHINE_CONTROL_SIZE_KEY,
          JSON.stringify(
            machineControlSizing
          )
        );
      } catch (_) {}
    }

    function applyMachineControlSizing(
      persist = true
    ) {
      machineControlSizing.actionIcon =
        clampMachineControlSize(
          machineControlSizing.actionIcon,
          18,
          48,
          MACHINE_CONTROL_SIZE_DEFAULTS
            .actionIcon
        );

      machineControlSizing.powerButton =
        clampMachineControlSize(
          machineControlSizing.powerButton,
          36,
          64,
          MACHINE_CONTROL_SIZE_DEFAULTS
            .powerButton
        );

      const page = $('page-home');

      if (page) {
        page.style.setProperty(
          '--machine-action-icon-size',
          machineControlSizing.actionIcon +
            'px'
        );
        page.style.setProperty(
          '--machine-power-button-size',
          machineControlSizing.powerButton +
            'px'
        );
      }

      syncMachineControlSizeInputs();

      if (persist) {
        storeMachineControlSizing();
      }
    }

    function loadMachineControlSizing() {
      machineControlSizing = {
        ...MACHINE_CONTROL_SIZE_DEFAULTS
      };

      try {
        const raw = localStorage.getItem(
          MACHINE_CONTROL_SIZE_KEY
        );

        if (raw) {
          const saved = JSON.parse(raw);

          if (
            saved &&
            typeof saved === 'object'
          ) {
            machineControlSizing = {
              ...machineControlSizing,
              ...saved
            };
          }
        }
      } catch (_) {}

      applyMachineControlSizing(false);
    }

    function setMachineActionIconSize(value) {
      machineControlSizing.actionIcon =
        value;
      applyMachineControlSizing();
    }

    function setMachinePowerButtonSize(value) {
      machineControlSizing.powerButton =
        value;
      applyMachineControlSizing();
    }

    function resetMachineControlSizing() {
      machineControlSizing = {
        ...MACHINE_CONTROL_SIZE_DEFAULTS
      };
      applyMachineControlSizing();
    }

    function initMachineControlSizing() {
      loadMachineControlSizing();
    }

    function homeTopPanelShareBounds(grid) {
      const usableWidth = Math.max(
        1,
        (grid?.clientWidth || 0) - 12
      );
      const compact = matchMedia(
        '(max-width:840px)'
      ).matches;
      const minimumMachine =
        compact ? 350 : 430;
      const minimumMetrics =
        compact ? 290 : 330;

      let minimum = Math.max(
        38,
        minimumMachine /
          usableWidth *
          100
      );
      let maximum = Math.min(
        72,
        100 -
          minimumMetrics /
          usableWidth *
          100
      );

      if (
        !Number.isFinite(minimum) ||
        !Number.isFinite(maximum) ||
        minimum > maximum
      ) {
        minimum = 48;
        maximum = 60;
      }

      return { minimum, maximum };
    }

    function clampHomeTopMachineShare(
      value,
      grid = document.querySelector(
        '.home-top-grid'
      )
    ) {
      const numeric = Number(value);
      const bounds =
        homeTopPanelShareBounds(grid);

      if (!Number.isFinite(numeric)) {
        return Math.max(
          bounds.minimum,
          Math.min(
            bounds.maximum,
            HOME_TOP_MACHINE_SHARE_DEFAULT
          )
        );
      }

      return Math.max(
        bounds.minimum,
        Math.min(
          bounds.maximum,
          numeric
        )
      );
    }

    function updateHomeTopResizeValue() {
      const output = $('homeTopResizeValue');
      if (!output) return;
      const machine = Math.round(homeTopMachineShare);
      const metrics = 100 - machine;
      output.textContent = machine + ' / ' + metrics;
    }

            function applyHomeTopPanelSize(
      value,
      persist = true
    ) {
      const grid = document.querySelector(
        '.home-top-grid'
      );

      if (!grid) return;

      grid.style.removeProperty(
        'grid-template-columns'
      );
      grid.style.removeProperty(
        '--home-top-machine-track'
      );
      grid.style.removeProperty(
        '--home-top-metrics-track'
      );
      grid.style.removeProperty(
        '--home-top-machine-share'
      );
      grid.style.removeProperty(
        '--home-top-metrics-share'
      );

      requestAnimationFrame(drawTemp);
    }

        function loadHomeTopPanelSize() {
      let value =
        HOME_TOP_MACHINE_SHARE_DEFAULT;

      try {
        const raw = localStorage.getItem(
          HOME_TOP_PANEL_SIZE_KEY
        );

        if (
          raw !== null &&
          String(raw).trim() !== ''
        ) {
          const saved = Number(raw);

          if (Number.isFinite(saved)) {
            value = saved;
          }
        }
      } catch (_) {}

      applyHomeTopPanelSize(
        value,
        false
      );
    }

    function resetHomeTopPanelSize() {
      applyHomeTopPanelSize(HOME_TOP_MACHINE_SHARE_DEFAULT);
    }

    function updateHomeTopResizeLabel() {
      const handle = $('homeTopResizeHandle');
      if (!handle) return;
      const label = tr('Изменить ширину верхних карточек');
      handle.title =
        label + ' · ' +
        tr('Двойной щелчок — стандартный размер');
      handle.setAttribute('aria-label', label);
    }

    function ensureHomeTopResizeHandle() {
      const grid = document.querySelector('.home-top-grid');
      if (!grid) return;

      let handle = $('homeTopResizeHandle');
      if (!handle) {
        handle = document.createElement('button');
        handle.id = 'homeTopResizeHandle';
        handle.type = 'button';
        handle.className = 'home-top-resize-handle';
        handle.innerHTML =
          '<span aria-hidden="true"></span>' +
          '<output id="homeTopResizeValue">56 / 44</output>';
        handle.addEventListener('pointerdown', beginHomeTopResize);
        handle.addEventListener('dblclick', event => {
          event.preventDefault();
          event.stopPropagation();
          resetHomeTopPanelSize();
        });
        grid.appendChild(handle);
      }

      updateHomeTopResizeLabel();
      updateHomeTopResizeValue();
    }

    function beginHomeTopResize(event) {
      if (
        !homeDashboardEditing ||
        event.button > 0 ||
        matchMedia('(max-width:700px)').matches
      ) return;

      const handle = event.currentTarget;
      homeTopResizeHandle = handle;
      homeTopResizePointerId = event.pointerId;
      document.body.classList.add('home-top-resizing');
      try { handle.setPointerCapture(event.pointerId); } catch (_) {}
      event.preventDefault();
      event.stopPropagation();
    }

    function moveHomeTopResize(event) {
      if (
        !homeTopResizeHandle ||
        homeTopResizePointerId !== event.pointerId
      ) return;

      const grid = document.querySelector('.home-top-grid');
      if (!grid) return;

      const rect = grid.getBoundingClientRect();
      const usableWidth = Math.max(1, rect.width - 12);
      const metricsFirst = grid.classList.contains('metrics-first');
      const machinePixels = metricsFirst
        ? rect.right - event.clientX - 6
        : event.clientX - rect.left - 6;
      const share = machinePixels / usableWidth * 100;

      applyHomeTopPanelSize(share, false);
      event.preventDefault();
      event.stopPropagation();
    }

    function finishHomeTopResize(event) {
      if (
        !homeTopResizeHandle ||
        (
          event &&
          homeTopResizePointerId !== null &&
          event.pointerId !== homeTopResizePointerId
        )
      ) return;

      const handle = homeTopResizeHandle;
      const pointerId = homeTopResizePointerId;
      if (
        handle &&
        pointerId !== null &&
        handle.hasPointerCapture?.(pointerId)
      ) {
        try { handle.releasePointerCapture(pointerId); } catch (_) {}
      }

      homeTopResizeHandle = null;
      homeTopResizePointerId = null;
      document.body.classList.remove('home-top-resizing');
      applyHomeTopPanelSize(homeTopMachineShare);
    }

    function syncHomeTopPanelSizeToViewport() {
      const grid = document.querySelector(
        '.home-top-grid'
      );

      if (!grid) return;

      grid.style.removeProperty(
        'grid-template-columns'
      );
      grid.style.removeProperty(
        '--home-top-machine-track'
      );
      grid.style.removeProperty(
        '--home-top-metrics-track'
      );
    }

    function homeTopPanels() {
      const grid = document.querySelector(
        '.home-top-grid'
      );

      return grid
        ? [
            ...grid.querySelectorAll(
              ':scope > [data-home-top-panel]'
            )
          ]
        : [];
    }

    function updateHomeTopGridDirection() {
      const grid = document.querySelector(
        '.home-top-grid'
      );

      if (!grid) return;

      const first = grid.querySelector(
        ':scope > [data-home-top-panel]'
      );

      grid.classList.toggle(
        'metrics-first',
        first?.dataset.homeTopPanel ===
          'metrics'
      );

      applyHomeTopPanelSize(
        homeTopMachineShare,
        false
      );
    }

    function saveHomeTopPanelOrder() {
      const order = homeTopPanels().map(
        panel => panel.dataset.homeTopPanel
      );

      try {
        localStorage.setItem(
          HOME_TOP_PANEL_ORDER_KEY,
          JSON.stringify(order)
        );
      } catch (_) {}
    }

    function applyHomeTopPanelOrder(order) {
      const grid = document.querySelector(
        '.home-top-grid'
      );

      if (!grid) return;

      const valid = Array.isArray(order)
        ? order.filter(
            (id, index, list) =>
              HOME_TOP_PANEL_DEFAULT_ORDER.includes(
                id
              ) &&
              list.indexOf(id) === index
          )
        : [];

      const resolved = [
        ...valid,
        ...HOME_TOP_PANEL_DEFAULT_ORDER.filter(
          id => !valid.includes(id)
        )
      ];

      resolved.forEach(id => {
        const panel = grid.querySelector(
          ':scope > [data-home-top-panel="' +
            id +
          '"]'
        );

        if (panel) {
          grid.appendChild(panel);
        }
      });

      updateHomeTopGridDirection();

      requestAnimationFrame(() => {
        drawTemp();
      });
    }

    function loadHomeTopPanelOrder() {
      let order =
        HOME_TOP_PANEL_DEFAULT_ORDER;

      try {
        const saved = JSON.parse(
          localStorage.getItem(
            HOME_TOP_PANEL_ORDER_KEY
          ) || 'null'
        );

        if (Array.isArray(saved)) {
          order = saved;
        }
      } catch (_) {}

      applyHomeTopPanelOrder(order);
    }

    function ensureHomeTopPanelHandles() {
      homeTopPanels().forEach(panel => {
        if (
          panel.querySelector(
            ':scope > .home-top-panel-order-handle'
          )
        ) {
          return;
        }

        const handle =
          document.createElement('button');

        handle.type = 'button';
        handle.className =
          'home-top-panel-order-handle';
        handle.textContent = '⠿';
        handle.addEventListener(
          'pointerdown',
          beginHomeTopPanelDrag
        );

        panel.insertBefore(
          handle,
          panel.firstChild
        );
      });

      updateHomeTopPanelHandleLabels();
    }

    function updateHomeTopPanelHandleLabels() {
      document
        .querySelectorAll(
          '.home-top-panel-order-handle'
        )
        .forEach(handle => {
          const label = tr(
            'Перетащить верхнюю карточку'
          );

          handle.title = label;
          handle.setAttribute(
            'aria-label',
            label
          );
        });
    }

    function beginHomeTopPanelDrag(event) {
      if (
        !homeDashboardEditing ||
        event.button > 0
      ) {
        return;
      }

      const handle = event.currentTarget;
      const panel = handle.closest(
        '[data-home-top-panel]'
      );

      if (!panel) return;

      homeTopPanelDragPanel = panel;
      homeTopPanelDragHandle = handle;
      homeTopPanelPointerId =
        event.pointerId;
      homeTopPanelPressStart = {
        x:event.clientX,
        y:event.clientY
      };
      homeTopPanelMoved = false;
      homeTopPanelSwapped = false;

      panel.classList.add(
        'home-top-panel-dragging'
      );
      document.body.classList.add(
        'home-top-panel-layout-dragging'
      );

      try {
        handle.setPointerCapture(
          event.pointerId
        );
      } catch (_) {}

      event.preventDefault();
      event.stopPropagation();
    }

        function moveHomeTopPanelDrag(event) {
      if (
        !homeTopPanelDragPanel ||
        homeTopPanelPointerId !==
          event.pointerId
      ) {
        return;
      }

      const dx =
        event.clientX -
        homeTopPanelPressStart.x;
      const dy =
        event.clientY -
        homeTopPanelPressStart.y;

      /*
       * There are only two cards, so a clear horizontal gesture can
       * swap them immediately. Vertical movement is ignored.
       */
      const horizontalGesture =
        Math.abs(dx) >= 24 &&
        Math.abs(dx) > Math.abs(dy);

      if (!horizontalGesture) {
        return;
      }

      homeTopPanelMoved = true;

      if (!homeTopPanelSwapped) {
        const grid = document.querySelector(
          '.home-top-grid'
        );
        const panels = homeTopPanels();

        if (
          grid &&
          panels.length === 2
        ) {
          grid.insertBefore(
            panels[1],
            panels[0]
          );

          homeTopPanelSwapped = true;
          updateHomeTopGridDirection();
          requestAnimationFrame(drawTemp);
        }
      }

      event.preventDefault();
      event.stopPropagation();
    }

    function swapHomeTopPanels() {
      const grid = document.querySelector(
        '.home-top-grid'
      );
      const panels = homeTopPanels();

      if (
        !grid ||
        panels.length !== 2
      ) {
        return;
      }

      grid.insertBefore(
        panels[1],
        panels[0]
      );

      updateHomeTopGridDirection();
      saveHomeTopPanelOrder();
      requestAnimationFrame(drawTemp);
    }

    function finishHomeTopPanelDrag(event) {
      if (
        !homeTopPanelDragPanel ||
        (
          event &&
          homeTopPanelPointerId !== null &&
          event.pointerId !==
            homeTopPanelPointerId
        )
      ) {
        return;
      }

      const panel =
        homeTopPanelDragPanel;
      const handle =
        homeTopPanelDragHandle;
      const pointerId =
        homeTopPanelPointerId;
      const moved =
        homeTopPanelMoved;

      panel?.classList.remove(
        'home-top-panel-dragging'
      );
      document.body.classList.remove(
        'home-top-panel-layout-dragging'
      );

      if (
        handle &&
        pointerId !== null &&
        handle.hasPointerCapture?.(
          pointerId
        )
      ) {
        try {
          handle.releasePointerCapture(
            pointerId
          );
        } catch (_) {}
      }

      homeTopPanelDragPanel = null;
      homeTopPanelDragHandle = null;
      homeTopPanelPointerId = null;
      homeTopPanelPressStart = null;
      homeTopPanelMoved = false;
      homeTopPanelSwapped = false;

      if (!moved) {
        swapHomeTopPanels();
      } else {
        saveHomeTopPanelOrder();
        requestAnimationFrame(drawTemp);
      }
    }

            function initHomeTopPanelOrder() {
      const grid = document.querySelector(
        '.home-top-grid'
      );

      if (!grid) return;

      grid.dataset.dragReady = '1';
      grid.classList.remove(
        'metrics-first'
      );
      grid.style.removeProperty(
        'grid-template-columns'
      );
      grid.style.removeProperty(
        '--home-top-machine-track'
      );
      grid.style.removeProperty(
        '--home-top-metrics-track'
      );
      grid.style.removeProperty(
        '--home-top-machine-share'
      );
      grid.style.removeProperty(
        '--home-top-metrics-share'
      );

      grid
        .querySelectorAll(
          '.home-top-panel-order-handle,' +
          '.home-top-resize-handle'
        )
        .forEach(node => node.remove());

      document.body.classList.remove(
        'home-top-resizing',
        'home-top-panel-layout-dragging'
      );
    }

    function homeInfoCards() {
      const grid = document.querySelector('.home-top-grid');
      return grid ? [...grid.querySelectorAll(':scope > [data-home-info-card]')] : [];
    }

    function saveHomeInfoCardOrder() {
      try {
        localStorage.setItem(HOME_INFO_CARD_ORDER_KEY,
          JSON.stringify(homeInfoCards().map(card => card.dataset.homeInfoCard)));
      } catch (_) {}
    }

    function updateHomeInfoCardTracks() {
      const grid = document.querySelector('.home-top-grid');
      if (!grid) return;
      const tracks = {
        pressure:'minmax(145px,var(--home-pressure-column-share,18fr))',
        stats:'minmax(145px,var(--home-stats-column-share,18fr))',
        temperature:'minmax(210px,var(--home-right-column-share,26fr))'
      };
      const infoTracks = homeInfoCards()
        .map(card => tracks[card.dataset.homeInfoCard])
        .filter(Boolean);
      grid.style.setProperty(
        '--home-top-grid-columns',
        ['minmax(360px,var(--home-left-column-share,38fr))', ...infoTracks].join(' ')
      );
    }

    function applyHomeInfoCardOrder(order) {
      const grid = document.querySelector('.home-top-grid');
      if (!grid) return;
      const valid = Array.isArray(order)
        ? order.filter((id, index, list) =>
            HOME_INFO_CARD_DEFAULT_ORDER.includes(id) && list.indexOf(id) === index)
        : [];
      [...valid, ...HOME_INFO_CARD_DEFAULT_ORDER.filter(id => !valid.includes(id))]
        .forEach(id => {
          const card = grid.querySelector(':scope > [data-home-info-card="' + id + '"]');
          if (card) grid.appendChild(card);
        });
      updateHomeInfoCardTracks();
      requestAnimationFrame(() => { drawHomePressureGauge(); drawHomeTemperatureMini(); drawTemp(); });
    }

    function beginHomeInfoCardDrag(event) {
      if (!homeDashboardEditing || event.button > 0) return;
      const handle = event.currentTarget;
      const card = handle.closest('[data-home-info-card]');
      if (!card) return;
      homeInfoCardDrag = card;
      homeInfoCardHandle = handle;
      homeInfoCardPointerId = event.pointerId;
      card.classList.add('home-info-card-dragging');
      document.body.classList.add('home-info-card-layout-dragging');
      try { handle.setPointerCapture(event.pointerId); } catch (_) {}
      event.preventDefault();
      event.stopPropagation();
    }

    function moveHomeInfoCardDrag(event) {
      if (!homeInfoCardDrag || homeInfoCardPointerId !== event.pointerId) return;
      event.preventDefault();
      const grid = document.querySelector('.home-top-grid');
      const target = document.elementFromPoint(event.clientX, event.clientY)
        ?.closest?.('[data-home-info-card]');
      document.querySelectorAll('.home-info-card-drop-target')
        .forEach(node => node.classList.remove('home-info-card-drop-target'));
      if (!grid || !target || target === homeInfoCardDrag || target.parentElement !== grid) return;
      target.classList.add('home-info-card-drop-target');
      const rect = target.getBoundingClientRect();
      const after = matchMedia('(max-width:900px)').matches
        ? event.clientY > rect.top + rect.height / 2
        : event.clientX > rect.left + rect.width / 2;
      grid.insertBefore(homeInfoCardDrag, after ? target.nextSibling : target);
      updateHomeInfoCardTracks();
    }

    function finishHomeInfoCardDrag(event) {
      if (!homeInfoCardDrag ||
          (event && homeInfoCardPointerId !== null && event.pointerId !== homeInfoCardPointerId)) return;
      const handle = homeInfoCardHandle;
      const pointerId = homeInfoCardPointerId;
      homeInfoCardDrag.classList.remove('home-info-card-dragging');
      document.querySelectorAll('.home-info-card-drop-target')
        .forEach(node => node.classList.remove('home-info-card-drop-target'));
      document.body.classList.remove('home-info-card-layout-dragging');
      if (handle && pointerId !== null && handle.hasPointerCapture?.(pointerId)) {
        try { handle.releasePointerCapture(pointerId); } catch (_) {}
      }
      homeInfoCardDrag = null;
      homeInfoCardHandle = null;
      homeInfoCardPointerId = null;
      saveHomeInfoCardOrder();
      requestAnimationFrame(() => { drawHomePressureGauge(); drawHomeTemperatureMini(); drawTemp(); });
    }

    function initHomeInfoCardOrder() {
      let order = HOME_INFO_CARD_DEFAULT_ORDER;
      try {
        const saved = JSON.parse(localStorage.getItem(HOME_INFO_CARD_ORDER_KEY) || 'null');
        if (Array.isArray(saved)) order = saved;
      } catch (_) {}
      applyHomeInfoCardOrder(order);
      homeInfoCards().forEach(card => {
        let handle = card.querySelector(':scope > .home-info-order-handle');
        if (!handle) {
          handle = document.createElement('button');
          handle.type = 'button';
          handle.className = 'home-info-order-handle';
          handle.textContent = '⠿';
          handle.addEventListener('pointerdown', beginHomeInfoCardDrag);
          card.insertBefore(handle, card.firstChild);
        }
        const label = tr('Перетащить верхнюю карточку');
        handle.title = label;
        handle.setAttribute('aria-label', label);
      });
      document.addEventListener('pointermove', moveHomeInfoCardDrag, {passive:false});
      document.addEventListener('pointerup', finishHomeInfoCardDrag);
      document.addEventListener('pointercancel', finishHomeInfoCardDrag);
    }

    function shotProfileShareBounds(board) {
      const usableWidth = Math.max(
        1,
        (board?.clientWidth || 0) - 12
      );
      const minimumProfile = 255;
      const minimumChart = 420;

      let minimum = Math.max(
        25,
        minimumProfile /
          usableWidth *
          100
      );
      let maximum = Math.min(
        55,
        100 -
          minimumChart /
          usableWidth *
          100
      );

      if (
        !Number.isFinite(minimum) ||
        !Number.isFinite(maximum) ||
        minimum > maximum
      ) {
        minimum = 30;
        maximum = 48;
      }

      return {
        minimum,
        maximum
      };
    }

    function clampShotProfileShare(
      value,
      board = $('shotProfileBoard')
    ) {
      const numeric = Number(value);
      const bounds =
        shotProfileShareBounds(board);

      if (!Number.isFinite(numeric)) {
        return Math.max(
          bounds.minimum,
          Math.min(
            bounds.maximum,
            SHOT_PROFILE_DEFAULT_SHARE
          )
        );
      }

      return Math.max(
        bounds.minimum,
        Math.min(
          bounds.maximum,
          numeric
        )
      );
    }

    function updateShotProfileResizeValue() {
      const output = $(
        'shotProfileResizeValue'
      );

      if (!output) return;

      const profiles = Math.round(
        shotProfileShare
      );
      const chart = 100 - profiles;

      output.textContent =
        profiles + ' / ' + chart;
    }

    function applyShotProfilePanelSize(
      value,
      persist = true
    ) {
      const board = $('shotProfileBoard');
      if (!board) return;

      shotProfileShare =
        clampShotProfileShare(
          value,
          board
        );

      board.style.setProperty(
        '--shot-profile-share',
        shotProfileShare + 'fr'
      );
      board.style.setProperty(
        '--shot-chart-share',
        (100 - shotProfileShare) +
          'fr'
      );

      updateShotProfileResizeValue();

      if (persist) {
        try {
          localStorage.setItem(
            SHOT_PROFILE_PANEL_SIZE_KEY,
            String(shotProfileShare)
          );
        } catch (_) {}
      }

      requestAnimationFrame(draw);
    }

        function loadShotProfilePanelSize() {
      let value =
        SHOT_PROFILE_DEFAULT_SHARE;

      try {
        const raw = localStorage.getItem(
          SHOT_PROFILE_PANEL_SIZE_KEY
        );

        if (
          raw !== null &&
          String(raw).trim() !== ''
        ) {
          const saved = Number(raw);

          if (Number.isFinite(saved)) {
            value = saved;
          }
        }
      } catch (_) {}

      applyShotProfilePanelSize(
        value,
        false
      );
    }

    function resetShotProfilePanelSize() {
      applyShotProfilePanelSize(
        SHOT_PROFILE_DEFAULT_SHARE
      );
    }

    function updateShotProfileResizeLabel() {
      const handle = $(
        'shotProfileResizeHandle'
      );

      if (!handle) return;

      const label = tr(
        'Изменить ширину нижних карточек'
      );

      handle.title =
        label +
        ' · ' +
        tr(
          'Двойной щелчок — стандартный размер'
        );
      handle.setAttribute(
        'aria-label',
        label
      );
    }

    function ensureShotProfileResizeHandle() {
      const board = $('shotProfileBoard');
      if (!board) return;

      let handle = $(
        'shotProfileResizeHandle'
      );

      if (!handle) {
        handle =
          document.createElement('button');
        handle.id =
          'shotProfileResizeHandle';
        handle.type = 'button';
        handle.className =
          'shot-profile-resize-handle';
        handle.innerHTML =
          '<span aria-hidden="true"></span>' +
          '<output id="shotProfileResizeValue">' +
          '36 / 64' +
          '</output>';

        handle.addEventListener(
          'pointerdown',
          beginShotProfileResize
        );
        handle.addEventListener(
          'dblclick',
          event => {
            event.preventDefault();
            event.stopPropagation();
            resetShotProfilePanelSize();
          }
        );

        board.appendChild(handle);
      }

      updateShotProfileResizeLabel();
      updateShotProfileResizeValue();
    }

    function beginShotProfileResize(event) {
      if (
        !homeDashboardEditing ||
        event.button > 0 ||
        matchMedia(
          '(max-width:980px)'
        ).matches
      ) {
        return;
      }

      const handle = event.currentTarget;

      shotProfileResizeHandle = handle;
      shotProfileResizePointerId =
        event.pointerId;

      document.body.classList.add(
        'shot-profile-resizing'
      );

      try {
        handle.setPointerCapture(
          event.pointerId
        );
      } catch (_) {}

      event.preventDefault();
      event.stopPropagation();
    }

    function moveShotProfileResize(event) {
      if (
        !shotProfileResizeHandle ||
        shotProfileResizePointerId !==
          event.pointerId
      ) {
        return;
      }

      const board = $('shotProfileBoard');
      if (!board) return;

      const rect =
        board.getBoundingClientRect();
      const usableWidth = Math.max(
        1,
        rect.width - 12
      );
      const profilesFirst =
        board.classList.contains(
          'profiles-first'
        );

      const profilePixels =
        profilesFirst
          ? event.clientX -
            rect.left -
            6
          : rect.right -
            event.clientX -
            6;

      const share =
        profilePixels /
        usableWidth *
        100;

      applyShotProfilePanelSize(
        share,
        false
      );

      event.preventDefault();
      event.stopPropagation();
    }

    function finishShotProfileResize(event) {
      if (
        !shotProfileResizeHandle ||
        (
          event &&
          shotProfileResizePointerId !==
            null &&
          event.pointerId !==
            shotProfileResizePointerId
        )
      ) {
        return;
      }

      const handle =
        shotProfileResizeHandle;
      const pointerId =
        shotProfileResizePointerId;

      if (
        handle &&
        pointerId !== null &&
        handle.hasPointerCapture?.(
          pointerId
        )
      ) {
        try {
          handle.releasePointerCapture(
            pointerId
          );
        } catch (_) {}
      }

      shotProfileResizeHandle = null;
      shotProfileResizePointerId = null;
      document.body.classList.remove(
        'shot-profile-resizing'
      );

      applyShotProfilePanelSize(
        shotProfileShare
      );
    }

    function syncShotProfileSizeToViewport() {
      applyShotProfilePanelSize(
        shotProfileShare,
        false
      );
    }

    function shotProfilePanels() {
      const board = $('shotProfileBoard');
      return board
        ? [...board.querySelectorAll(':scope > [data-shot-panel]')]
        : [];
    }

    function updateShotProfileBoardDirection() {
      const board = $('shotProfileBoard');
      if (!board) return;

      const first = board.querySelector(':scope > [data-shot-panel]');
      board.classList.toggle(
        'profiles-first',
        first?.dataset.shotPanel === 'profiles'
      );

      applyShotProfilePanelSize(
        shotProfileShare,
        false
      );
    }

    function saveShotProfilePanelOrder() {
      const order = shotProfilePanels().map(panel => panel.dataset.shotPanel);
      try {
        localStorage.setItem(
          SHOT_PROFILE_PANEL_ORDER_KEY,
          JSON.stringify(order)
        );
      } catch (_) {}
    }

    function applyShotProfilePanelOrder(order) {
      const board = $('shotProfileBoard');
      if (!board) return;

      const allowed = ['chart', 'profiles'];
      const valid = Array.isArray(order)
        ? order.filter(
            (id, index, list) =>
              allowed.includes(id) &&
              list.indexOf(id) === index
          )
        : [];
      const resolved = [
        ...valid,
        ...allowed.filter(id => !valid.includes(id))
      ];

      resolved.forEach(id => {
        const panel = board.querySelector(
          ':scope > [data-shot-panel="' + id + '"]'
        );
        if (panel) board.appendChild(panel);
      });

      updateShotProfileBoardDirection();
      requestAnimationFrame(draw);
    }

        function loadShotProfilePanelOrder() {
      let order = ['profiles', 'chart'];

      try {
        const saved = JSON.parse(
          localStorage.getItem(
            SHOT_PROFILE_PANEL_ORDER_KEY
          ) || 'null'
        );

        if (Array.isArray(saved)) {
          order = saved;
        }
      } catch (_) {}

      applyShotProfilePanelOrder(order);
    }

    function createShotPanelHandle(panel, host) {
      if (!panel || !host || host.querySelector('.shot-panel-order-handle')) {
        return;
      }

      const handle = document.createElement('button');
      handle.type = 'button';
      handle.className = 'small shot-panel-order-handle';
      handle.textContent = '⠿';
      handle.title = tr('Перетащить карточку');
      handle.setAttribute('aria-label', tr('Перетащить карточку'));
      handle.addEventListener('pointerdown', beginShotPanelDrag);
      host.prepend(handle);
    }

        function ensureShotProfilePanelHandles() {
      const chart = $('shotChartCard');
      const profiles = $('profileLibraryCard');

      createShotPanelHandle(
        chart,
        chart?.querySelector(
          '.chart-head-actions'
        )
      );

      createShotPanelHandle(
        profiles,
        profiles?.querySelector(
          '.home-shot-rail-actions'
        )
      );
    }

    function updateShotPanelHandleLabels() {
      document
        .querySelectorAll(
          '.shot-panel-order-handle'
        )
        .forEach(handle => {
          const label = tr(
            'Перетащить карточку'
          );
          handle.title = label;
          handle.setAttribute(
            'aria-label',
            label
          );
        });
    }

    function beginShotPanelDrag(event) {
      if (
        event.button > 0 ||
        !homeDashboardEditing ||
        shotRunning ||
        mainCurveDragKey
      ) return;

      const handle = event.currentTarget;
      const panel = handle.closest('[data-shot-panel]');
      if (!panel) return;

      shotPanelDragPanel = panel;
      shotPanelDragHandle = handle;
      shotPanelPointerId = event.pointerId;
      shotPanelPressStart = {
        x:event.clientX,
        y:event.clientY
      };
      shotPanelMoved = false;
      shotPanelSwapped = false;

      panel.classList.add('shot-panel-dragging');
      document.body.classList.add('shot-panel-layout-dragging');

      try {
        handle.setPointerCapture(event.pointerId);
      } catch (_) {}

      event.preventDefault();
      event.stopPropagation();
    }

    function moveShotPanelDrag(event) {
      if (
        !shotPanelDragPanel ||
        shotPanelPointerId !== event.pointerId
      ) {
        return;
      }

      const dx = event.clientX - shotPanelPressStart.x;
      const dy = event.clientY - shotPanelPressStart.y;

      if (!shotPanelMoved && Math.hypot(dx, dy) > 8) {
        shotPanelMoved = true;
      }

      if (!shotPanelMoved || shotPanelSwapped) return;

      const board = $('shotProfileBoard');
      const underPointer = document.elementFromPoint(
        event.clientX,
        event.clientY
      );
      const target = underPointer?.closest?.('[data-shot-panel]');

      if (
        !board ||
        !target ||
        target === shotPanelDragPanel ||
        target.parentElement !== board
      ) {
        return;
      }

      if (shotPanelDragPanel.nextElementSibling === target) {
        board.insertBefore(target, shotPanelDragPanel);
      } else {
        board.insertBefore(shotPanelDragPanel, target);
      }

      shotPanelSwapped = true;
      updateShotProfileBoardDirection();
      requestAnimationFrame(draw);
      event.preventDefault();
    }

    function swapShotProfilePanels() {
      const board = $('shotProfileBoard');
      const panels = shotProfilePanels();
      if (!board || panels.length !== 2) return;

      board.insertBefore(panels[1], panels[0]);
      updateShotProfileBoardDirection();
      saveShotProfilePanelOrder();
      requestAnimationFrame(draw);
    }

    function finishShotPanelDrag(event) {
      if (
        !shotPanelDragPanel ||
        (
          event &&
          shotPanelPointerId !== null &&
          event.pointerId !== shotPanelPointerId
        )
      ) {
        return;
      }

      const panel = shotPanelDragPanel;
      const handle = shotPanelDragHandle;
      const pointerId = shotPanelPointerId;
      const moved = shotPanelMoved;

      panel?.classList.remove('shot-panel-dragging');
      document.body.classList.remove('shot-panel-layout-dragging');

      if (
        handle &&
        pointerId !== null &&
        handle.hasPointerCapture?.(pointerId)
      ) {
        try {
          handle.releasePointerCapture(pointerId);
        } catch (_) {}
      }

      shotPanelDragPanel = null;
      shotPanelDragHandle = null;
      shotPanelPointerId = null;
      shotPanelPressStart = null;
      shotPanelMoved = false;
      shotPanelSwapped = false;

      if (!moved) {
        swapShotProfilePanels();
      } else {
        saveShotProfilePanelOrder();
        requestAnimationFrame(draw);
      }
    }

    function initShotProfileBoard() {
      initHomeShotRail();
    }

    function homeDashboardBlocks() {
      const layout = $('homeDashboardLayout');
      return layout ? [...layout.querySelectorAll(':scope > [data-dashboard-block]')] : [];
    }

    function saveHomeDashboardLayout() {
      const order = homeDashboardBlocks().map(block => block.dataset.dashboardBlock);
      try {
        localStorage.setItem(HOME_DASHBOARD_LAYOUT_KEY, JSON.stringify(order));
        localStorage.setItem(HOME_DASHBOARD_VISIBILITY_KEY, JSON.stringify([...homeDashboardHidden]));
      } catch (_) {}
    }

    function syncShotBoardLabelPosition() {
      const layout = $('homeDashboardLayout');
      const label = document.querySelector('#page-home .shot-board-label');
      const shotBoard = $('shotProfileBoard');
      if (!layout || !label || !shotBoard) return;

      // This label belongs to the shot-profile section, so keep it directly
      // above that section even when the dashboard blocks are re-ordered.
      layout.insertBefore(label, shotBoard);
      label.hidden = shotBoard.hidden;
    }

    function applyHomeDashboardOrder(order) {
      const layout = $('homeDashboardLayout');
      if (!layout) return;
      const valid = Array.isArray(order)
        ? order.filter((id, index, list) => HOME_DASHBOARD_DEFAULT_ORDER.includes(id) && list.indexOf(id) === index)
        : [];
      const resolved = [...valid, ...HOME_DASHBOARD_DEFAULT_ORDER.filter(id => !valid.includes(id))];
      resolved.forEach(id => {
        const block = layout.querySelector('[data-dashboard-block="' + id + '"]');
        if (block) layout.appendChild(block);
      });
      syncShotBoardLabelPosition();
      requestAnimationFrame(() => { draw(); drawTemp(); });
    }

    function renderHomeHiddenBlocks() {
      const panel = $('homeHiddenBlocks');
      const list = $('homeHiddenBlockList');
      if (!panel || !list) return;
      list.innerHTML = '';
      [...homeDashboardHidden].filter(id => HOME_DASHBOARD_DEFAULT_ORDER.includes(id)).forEach(id => {
        const button = document.createElement('button');
        button.type = 'button';
        button.textContent = '+ ' + tr(HOME_DASHBOARD_TITLES[id] || id);
        button.onclick = () => restoreHomeDashboardBlock(id);
        list.appendChild(button);
      });
      panel.hidden = !homeDashboardEditing || !list.children.length;
    }

    function applyHomeDashboardVisibility() {
      homeDashboardBlocks().forEach(block => {
        block.hidden = homeDashboardHidden.has(block.dataset.dashboardBlock);
      });
      syncShotBoardLabelPosition();
      renderHomeHiddenBlocks();
      requestAnimationFrame(() => { draw(); drawTemp(); });
    }

    function hideHomeDashboardBlock(id) {
      if (!HOME_DASHBOARD_DEFAULT_ORDER.includes(id)) return;
      homeDashboardHidden.add(id);
      applyHomeDashboardVisibility();
      saveHomeDashboardLayout();
    }

    function restoreHomeDashboardBlock(id) {
      homeDashboardHidden.delete(id);
      applyHomeDashboardVisibility();
      saveHomeDashboardLayout();
    }

    function loadHomeDashboardLayout() {
      let order = HOME_DASHBOARD_DEFAULT_ORDER;
      try {
        const saved = JSON.parse(localStorage.getItem(HOME_DASHBOARD_LAYOUT_KEY) || 'null');
        const hidden = JSON.parse(localStorage.getItem(HOME_DASHBOARD_VISIBILITY_KEY) || 'null');
        if (Array.isArray(saved)) order = saved;
        if (Array.isArray(hidden)) {
          homeDashboardHidden = new Set(hidden.filter(id => HOME_DASHBOARD_DEFAULT_ORDER.includes(id)));
        }
      } catch (_) {}
      applyHomeDashboardOrder(order);
      applyHomeDashboardVisibility();
    }

    function ensureHomeDashboardControls(block) {
      if (!block) return;
      if (!block.querySelector(':scope > .dashboard-drag-handle')) {
        const handle = document.createElement('button');
        handle.type = 'button';
        handle.className = 'dashboard-drag-handle';
        handle.textContent = '⠿';
        handle.title = tr('Перетащить блок');
        handle.setAttribute('aria-label', tr('Перетащить блок'));
        handle.addEventListener('pointerdown', beginHomeDashboardPress);
        block.insertBefore(handle, block.firstChild);
      }
      if (!block.querySelector(':scope > .dashboard-hide-button')) {
        const hide = document.createElement('button');
        hide.type = 'button';
        hide.className = 'dashboard-hide-button';
        hide.title = tr('Скрыть блок');
        hide.setAttribute('aria-label', tr('Скрыть блок'));
        hide.innerHTML = '<svg viewBox="0 0 24 24" aria-hidden="true"><path d="M3 12s3.4-5 9-5 9 5 9 5-3.4 5-9 5-9-5-9-5Z"/><circle cx="12" cy="12" r="2.2"/><path d="m4 4 16 16"/></svg>';
        hide.addEventListener('click', event => {
          event.preventDefault();
          event.stopPropagation();
          hideHomeDashboardBlock(block.dataset.dashboardBlock);
        });
        block.appendChild(hide);
      }
    }

        function initHomeDashboardLayout() {
      const page = $('page-home');
      if (!page || $('homeDashboardLayout')) {
        document.body.style.visibility = '';
        return;
      }

      const overview = page.querySelector('.home-overview');
      const shotBoard = $('shotProfileBoard');
      const legacyGrid =
        shotBoard?.parentElement?.classList.contains('grid')
          ? shotBoard.parentElement
          : null;

      if (!overview || !shotBoard) {
        document.body.style.visibility = '';
        return;
      }

      initShotProfileBoard();

      const layout = document.createElement('div');
      layout.id = 'homeDashboardLayout';
      layout.className = 'home-dashboard-layout';

      const toolbar = page.querySelector('.home-layout-toolbar');

      if (toolbar) {
        toolbar.insertAdjacentElement('afterend', layout);
      } else {
        page.insertBefore(layout, page.firstChild);
      }

      [
        ['overview', overview, true],
        ['shot', shotBoard, true]
      ].forEach(([id, block, wide]) => {
        block.dataset.dashboardBlock = id;
        block.dataset.dashboardTitle =
          HOME_DASHBOARD_TITLES[id] || id;
        block.classList.add('dashboard-block');
        block.classList.toggle(
          'dashboard-block-wide',
          !!wide
        );
        ensureHomeDashboardControls(block);
        layout.appendChild(block);
      });

      if (legacyGrid && !legacyGrid.children.length) {
        legacyGrid.remove();
      }

      loadHomeDashboardLayout();
      initHomeTopPanelOrder();
      initMachineControlSizing();
      loadHomeSectionGap();
      loadHomeTemperatureCardHeight();
      loadHomeTopColumnSplit();
      applyTypographyZoneVariables();

      requestAnimationFrame(() => {
        syncHomeTopPanelSizeToViewport();
        syncShotProfileSizeToViewport();
        document.body.style.visibility = '';
      });

      document.addEventListener(
        'pointermove',
        moveHomeDashboardDrag,
        { passive:false }
      );
      document.addEventListener(
        'pointerup',
        finishHomeDashboardDrag
      );
      document.addEventListener(
        'pointercancel',
        finishHomeDashboardDrag
      );
    }

    function setHomeDashboardEditing(enabled) {
      homeDashboardEditing = !!enabled;
      const page = $('page-home');
      page?.classList.toggle('home-layout-editing', homeDashboardEditing);
      const edit = $('homeLayoutEditButton');
      const tools = $('homeLayoutTools');
      if (edit) {
        const label = tr(homeDashboardEditing ? 'Завершить настройку экрана' : 'Настроить экран');
        edit.classList.toggle('active', homeDashboardEditing);
        edit.setAttribute('aria-pressed', homeDashboardEditing ? 'true' : 'false');
        edit.setAttribute('aria-label', label);
        edit.title = label;
      }
      if (tools) tools.hidden = !homeDashboardEditing;
      renderHomeHiddenBlocks();
      if (!homeDashboardEditing) {
        cancelHomeDashboardPress();
        finishHomeDashboardDrag();
        saveHomeDashboardLayout();
      }
    }

    function toggleHomeDashboardEdit() {
      setHomeDashboardEditing(!homeDashboardEditing);
      if (!homeDashboardEditing) toast(tr('Расположение сохранено'));
    }

    function resetHomeDashboardLayout() {
      try {
        localStorage.removeItem(HOME_DASHBOARD_LAYOUT_KEY);
        localStorage.removeItem(HOME_DASHBOARD_VISIBILITY_KEY);
        localStorage.removeItem(HOME_TOP_PANEL_ORDER_KEY);
        localStorage.removeItem(HOME_TOP_PANEL_SIZE_KEY);
        localStorage.removeItem(HOME_INFO_CARD_ORDER_KEY);
        localStorage.removeItem(SHOT_PROFILE_PANEL_ORDER_KEY);
        localStorage.removeItem(SHOT_PROFILE_PANEL_SIZE_KEY);
        localStorage.removeItem(HOME_SECTION_GAP_KEY);
        localStorage.removeItem(HOME_TEMPERATURE_CARD_HEIGHT_KEY);
        localStorage.removeItem(HOME_TOP_COLUMN_SPLIT_KEY);
      } catch (_) {}
      homeDashboardHidden = new Set();
      applyHomeDashboardOrder(HOME_DASHBOARD_DEFAULT_ORDER);
      applyHomeDashboardVisibility();
      applyShotProfilePanelOrder([
        'profiles',
        'chart'
      ]);
      resetShotProfilePanelSize();
      resetHomeSectionGap();
      resetHomeTemperatureCardHeight();
      resetHomeTopColumnSplit();
      applyHomeInfoCardOrder(HOME_INFO_CARD_DEFAULT_ORDER);
      toast(tr('Расположение сброшено'));
    }

    function clearHomeDashboardPressTimer() {
      if (homeDashboardPressTimer) clearTimeout(homeDashboardPressTimer);
      homeDashboardPressTimer = 0;
      homeDashboardPressStart = null;
    }

    function releaseHomeDashboardPointer() {
      if (
        homeDashboardDragHandle &&
        homeDashboardPointerId !== null &&
        homeDashboardDragHandle.hasPointerCapture?.(homeDashboardPointerId)
      ) {
        try { homeDashboardDragHandle.releasePointerCapture(homeDashboardPointerId); } catch (_) {}
      }
      homeDashboardDragHandle = null;
      homeDashboardPointerId = null;
    }

    function cancelHomeDashboardPress() {
      clearHomeDashboardPressTimer();
      if (!homeDashboardDragBlock) releaseHomeDashboardPointer();
    }

    function beginHomeDashboardPress(event) {
      if (!homeDashboardEditing || event.button > 0) return;
      event.preventDefault();
      event.stopPropagation();

      clearHomeDashboardPressTimer();
      releaseHomeDashboardPointer();

      const handle = event.currentTarget;
      const block = handle.closest('[data-dashboard-block]');
      if (!block) return;

      homeDashboardDragHandle = handle;
      homeDashboardPointerId = event.pointerId;
      homeDashboardPressStart = { x:event.clientX, y:event.clientY, block };

      try { handle.setPointerCapture(event.pointerId); } catch (_) {}

      if (event.pointerType === 'mouse' || event.pointerType === 'pen') {
        startHomeDashboardDrag(block);
      } else {
        homeDashboardPressTimer = setTimeout(() => {
          startHomeDashboardDrag(block);
          if (navigator.vibrate) navigator.vibrate(25);
        }, 280);
      }
    }

    function startHomeDashboardDrag(block) {
      if (!homeDashboardEditing || !block || homeDashboardPointerId === null) return;

      // Важно: здесь нельзя вызывать cancelHomeDashboardPress(),
      // потому что он освобождает pointerId до первого pointermove.
      clearHomeDashboardPressTimer();

      homeDashboardDragBlock = block;
      block.classList.add('dashboard-dragging');
      document.body.classList.add('dashboard-layout-dragging');
    }

    function moveHomeDashboardDrag(event) {
      if (homeDashboardPointerId === null || homeDashboardPointerId !== event.pointerId) return;

      if (!homeDashboardDragBlock) {
        if (homeDashboardPressStart) {
          const dx = event.clientX - homeDashboardPressStart.x;
          const dy = event.clientY - homeDashboardPressStart.y;
          if (Math.hypot(dx, dy) > 9) cancelHomeDashboardPress();
        }
        return;
      }

      event.preventDefault();
      const layout = $('homeDashboardLayout');
      if (!layout) return;

      const underPointer = document.elementFromPoint(event.clientX, event.clientY);
      const target = underPointer?.closest?.('[data-dashboard-block]');

      document.querySelectorAll('.dashboard-drop-target').forEach(node => {
        node.classList.remove('dashboard-drop-target');
      });

      if (!target || target === homeDashboardDragBlock || target.parentElement !== layout) return;

      target.classList.add('dashboard-drop-target');
      const rect = target.getBoundingClientRect();
      const insertBefore = event.clientY < rect.top + rect.height / 2;

      layout.insertBefore(
        homeDashboardDragBlock,
        insertBefore ? target : target.nextSibling
      );
      syncShotBoardLabelPosition();
    }

    function finishHomeDashboardDrag(event) {
      if (
        event &&
        homeDashboardPointerId !== null &&
        event.pointerId !== homeDashboardPointerId
      ) return;

      clearHomeDashboardPressTimer();

      document.querySelectorAll('.dashboard-drop-target').forEach(node => {
        node.classList.remove('dashboard-drop-target');
      });

      if (homeDashboardDragBlock) {
        homeDashboardDragBlock.classList.remove('dashboard-dragging');
        homeDashboardDragBlock = null;
        syncShotBoardLabelPosition();
        saveHomeDashboardLayout();
        requestAnimationFrame(() => {
          draw();
          drawTemp();
        });
      }

      document.body.classList.remove('dashboard-layout-dragging');
      releaseHomeDashboardPointer();
    }

    function setSettingsPanel(panel, persist = true) {
      const allowed = [
        'temperature',
        'pressure',
        'machine'
      ];

      settingsPanelCurrent = allowed.includes(panel)
        ? panel
        : 'machine';

      document
        .querySelectorAll('[data-settings-target]')
        .forEach(button => {
          const active =
            button.dataset.settingsTarget ===
            settingsPanelCurrent;

          button.classList.toggle('active', active);
          button.setAttribute(
            'aria-selected',
            String(active)
          );
        });

      document
        .querySelectorAll('[data-settings-panel]')
        .forEach(panelNode => {
          const active =
            panelNode.dataset.settingsPanel ===
            settingsPanelCurrent;

          panelNode.hidden = !active;
          panelNode.classList.toggle('active', active);
        });

      if (persist) {
        try {
          localStorage.setItem(
            SETTINGS_PANEL_STORAGE_KEY,
            settingsPanelCurrent
          );
        } catch (_) {}
      }
    }


    function setSettingsHelpItem(owner, open) {
      if (!owner) return;
      owner.dataset.helpOpen = open ? 'true' : 'false';
      const button = owner.querySelector(':scope .settings-info-button');
      if (button) {
        button.classList.toggle('active', open);
        button.setAttribute('aria-expanded', String(open));
      }
    }

    function toggleSettingsHelpItem(button) {
      const panel = button.closest('[data-settings-panel]');
      const owner = button.closest('.settings-help-owner');
      if (!panel || !owner) return;

      if (panel.classList.contains('help-all-open')) {
        panel.classList.remove('help-all-open');
        const allButton = panel.querySelector('.settings-help-all-button');
        if (allButton) {
          allButton.classList.remove('active');
          allButton.setAttribute('aria-expanded', 'false');
          allButton.textContent = tr('Пояснения');
        }
      }

      const opening = owner.dataset.helpOpen !== 'true';
      panel.querySelectorAll('.settings-help-owner[data-help-open="true"]').forEach(node => {
        if (node !== owner) setSettingsHelpItem(node, false);
      });
      setSettingsHelpItem(owner, opening);
    }

    function toggleSettingsHelpAll(button) {
      const panel = button.closest('[data-settings-panel]');
      if (!panel) return;

      const opening = !panel.classList.contains('help-all-open');
      panel.classList.toggle('help-all-open', opening);
      button.classList.toggle('active', opening);
      button.setAttribute('aria-expanded', String(opening));
      button.textContent = tr(opening ? 'Скрыть пояснения' : 'Пояснения');

      panel.querySelectorAll('.settings-help-owner').forEach(owner => {
        setSettingsHelpItem(owner, false);
      });
    }

    function prepareSettingsHelpOwner(small) {
      if (!small || small.classList.contains('settings-help-text')) return;

      const owner = small.closest('.temperature-settings-section-head, .pid-parameter-card, .temperature-setting-copy');
      if (!owner) return;

      small.classList.add('settings-help-text');
      owner.classList.add('settings-help-owner');
      owner.dataset.helpOpen = 'false';
    }

    function initSettingsHelp() {
      document.querySelectorAll('[data-settings-panel="temperature"], [data-settings-panel="pressure"], [data-settings-panel="machine"]').forEach(panel => {
        if (panel.dataset.helpReady === 'true') return;
        panel.dataset.helpReady = 'true';
        panel.classList.add('settings-help-panel');

        const head = panel.querySelector(':scope > .settings-panel-head');
        if (head && !head.querySelector('.settings-help-all-button')) {
          const button = document.createElement('button');
          button.type = 'button';
          button.className = 'settings-help-all-button';
          button.textContent = tr('Пояснения');
          button.setAttribute('aria-expanded', 'false');
          button.addEventListener('click', () => toggleSettingsHelpAll(button));
          head.appendChild(button);
        }

        panel.querySelectorAll('.temperature-settings-section-head > small, .pid-parameter-card > small, .temperature-setting-copy > small').forEach(prepareSettingsHelpOwner);
      });
    }


    function toggleServiceHelpAll(button) {
      const panel = button.closest('[data-maintenance-panel]');
      if (!panel) return;

      const opening = !panel.classList.contains('help-all-open');
      panel.classList.toggle('help-all-open', opening);
      button.classList.toggle('active', opening);
      button.setAttribute('aria-expanded', String(opening));
      button.textContent = tr(opening ? 'Скрыть пояснения' : 'Пояснения');
    }

    function initMaintenanceHelp() {
      document.querySelectorAll('#page-maintenance [data-maintenance-panel]').forEach(panel => {
        if (panel.dataset.helpReady === 'true') return;
        panel.dataset.helpReady = 'true';
        panel.classList.add('service-help-panel');

        const head = panel.querySelector(':scope > .service-panel-head');
        if (head && !head.querySelector('.settings-help-all-button')) {
          const button = document.createElement('button');
          button.type = 'button';
          button.className = 'settings-help-all-button';
          button.textContent = tr('Пояснения');
          button.setAttribute('aria-expanded', 'false');
          button.addEventListener('click', () => toggleServiceHelpAll(button));
          head.appendChild(button);
        }
      });
    }

    function initSettingsWorkspace() {
      try {
        const saved = localStorage.getItem(
          SETTINGS_PANEL_STORAGE_KEY
        );

        if (saved) {
          settingsPanelCurrent = saved;
        }
      } catch (_) {}

      setSettingsPanel(
        settingsPanelCurrent,
        false
      );
    }

    function setWorkspacePanel({
      targetSelector,
      panelSelector,
      value,
      allowed,
      storageKey,
      persist = true
    }) {
      const resolved = allowed.includes(value)
        ? value
        : allowed[0];

      document
        .querySelectorAll(targetSelector)
        .forEach(button => {
          const active =
            button.dataset[
              targetSelector.includes('maintenance')
                ? 'maintenanceTarget'
                : 'diagnosticsTarget'
            ] === resolved;

          button.classList.toggle('active', active);
          button.setAttribute(
            'aria-selected',
            String(active)
          );
        });

      document
        .querySelectorAll(panelSelector)
        .forEach(panel => {
          const current =
            panel.dataset[
              panelSelector.includes('maintenance')
                ? 'maintenancePanel'
                : 'diagnosticsPanel'
            ];
          const active = current === resolved;
          panel.hidden = !active;
          panel.classList.toggle('active', active);
        });

      if (persist) {
        try {
          localStorage.setItem(
            storageKey,
            resolved
          );
        } catch (_) {}
      }

      return resolved;
    }

    function setMaintenancePanel(
      panel,
      persist = true
    ) {
      maintenancePanelCurrent =
        setWorkspacePanel({
          targetSelector:
            '[data-maintenance-target]',
          panelSelector:
            '[data-maintenance-panel]',
          value:panel,
          allowed:[
            'scale',
            'backflush',
            'shotlog',
            'device'
          ],
          storageKey:
            MAINTENANCE_PANEL_STORAGE_KEY,
          persist
        });
    }

    function setDiagnosticsPanel(
      panel,
      persist = true
    ) {
      diagnosticsPanelCurrent =
        setWorkspacePanel({
          targetSelector:
            '[data-diagnostics-target]',
          panelSelector:
            '[data-diagnostics-panel]',
          value:panel,
          allowed:[
            'pressure',
            'temperature',
            'scale',
            'system'
          ],
          storageKey:
            DIAGNOSTICS_PANEL_STORAGE_KEY,
          persist
        });
    }

    function initServiceWorkspaces() {
      try {
        maintenancePanelCurrent =
          localStorage.getItem(
            MAINTENANCE_PANEL_STORAGE_KEY
          ) ||
          maintenancePanelCurrent;

        diagnosticsPanelCurrent =
          localStorage.getItem(
            DIAGNOSTICS_PANEL_STORAGE_KEY
          ) ||
          diagnosticsPanelCurrent;
      } catch (_) {}

      setMaintenancePanel(
        maintenancePanelCurrent,
        false
      );
      setDiagnosticsPanel(
        diagnosticsPanelCurrent,
        false
      );
    }

    function showPage(page) {
      if (page !== 'home') {
        toggleChartExpanded(false);
        setTemperatureDetails(false);
        toggleTypographyPanel(false);

        if (homeDashboardEditing) {
          setHomeDashboardEditing(false);
        }
      }

      document
        .querySelectorAll('.page')
        .forEach(node => {
          node.classList.toggle(
            'active',
            node.id === 'page-' + page
          );
        });

      document
        .querySelectorAll('.nav-item')
        .forEach(node => {
          node.classList.toggle(
            'active',
            node.dataset.page === page
          );
        });

      const layoutButton =
        $('homeLayoutEditButton');
      const fontButton =
        $('fontSettingsButton');

      if (layoutButton) {
        layoutButton.hidden = page !== 'home';
      }

      if (fontButton) {
        fontButton.hidden = page !== 'home';
      }

      closeSidebar();

      if (page === 'settings') {
        loadSettings(false);
      }

      if (page === 'maintenance') {
        loadMaintenanceSettings();
      }

      if (page === 'settings') {
        setSettingsPanel(
          settingsPanelCurrent,
          false
        );
      }

      if (page === 'maintenance') {
        setMaintenancePanel(
          maintenancePanelCurrent,
          false
        );
      }

      if (page === 'diagnostics') {
        setDiagnosticsPanel(
          diagnosticsPanelCurrent,
          false
        );
        updateXdbHumanStatus();
        void analyzeLatestShotCsv();
      }

      /*
       * Diagnostics uses SSE while healthy and only requests a REST status
       * snapshot when the event stream is unavailable.
       */
      if (page === 'diagnostics' && !eventStreamHealthy()) {
        refreshStatus();
      }

      if (page === 'history') {
        loadArchiveList();
      }

      if (page === 'lab') {
        void loadTrainingDataset(true);
      }

      if (page === 'home') {
        /*
         * Re-synchronize the home shot/profile workspace after it becomes
         * visible again. While another page is open settings can update
         * the selected profile while the home canvas has zero layout width.
         * syncHomeShotRailUi() restores profile/archive mode, and the nested
         * animation frame redraw waits until the canvas has its real size.
         */
        syncProfileControls(currentProfileName);
        syncHomeShotRailUi();
        updateShotScenario();
        updateChartControls();

        requestAnimationFrame(() => {
          draw();
          drawTemp();

          requestAnimationFrame(() => {
            draw();
            drawTemp();
          });
        });
      }
    }

    function updatePressureTarget() {
      const targetNode = $('target');
      const captionNode = $('targetCaption');
      if (!targetNode && !captionNode) return;

      const liveTarget = Number(values.target);
      const profileTarget = Number(profileCfg.mainPressure);
      const target = shotRunning && Number.isFinite(liveTarget) ? liveTarget : profileTarget;

      if (captionNode) captionNode.textContent = tr(shotRunning ? 'Цель сейчас' : 'Основная цель профиля');
      if (targetNode) targetNode.textContent = Number.isFinite(target) ? fmt(target) + ' ' + tr('бар') : tr('— бар');
    }

    function showPower(on, source = 'state') {
      const next = !!on;

      /*
       * Power commands are rendered optimistically so the control reacts on
       * the same frame as the click. While a command is in flight, ESPHome
       * can still deliver one stale SSE state from before the relay changed.
       * Ignore only that contradictory state for a short confirmation window;
       * the desired state itself confirms the command immediately.
       */
      if (source === 'state' && powerCommandPending) {
        if (next === powerDesiredState) {
          powerCommandPending = false;
          powerDesiredState = null;
          powerCommandStartedAt = 0;
          if (powerConfirmTimer) {
            clearTimeout(powerConfirmTimer);
            powerConfirmTimer = 0;
          }
        } else if (Date.now() - powerCommandStartedAt < 4000) {
          return;
        } else {
          powerCommandPending = false;
          powerDesiredState = null;
          powerCommandStartedAt = 0;
        }
      }

      machineOn = next;
      const button = $('powerButton');
      if (button) {
        button.classList.toggle('on', machineOn);
        button.classList.toggle('off', !machineOn);
        button.setAttribute('aria-label', tr(machineOn ? 'Выключить' : 'Включить'));
        button.setAttribute('aria-busy', powerCommandPending ? 'true' : 'false');
      }
      if ($('powerButtonLabel')) $('powerButtonLabel').textContent = tr(machineOn ? 'Включено' : 'Выключено');
      updateHomeDashboard(); updateQuickActions();
    }
    function showWaterLevel(on) { waterAvailable = on == null ? null : !!on; updateHomeDashboard(); }
    function setHotWater(on) { hotWaterOn = !!on; updateHomeDashboard(); updateQuickActions(); }
    function setSteamMode(on) { steamModeOn = !!on; updateHomeDashboard(); updateQuickActions(); drawTemp(); }
    function pressureSensorOk() { const n=$('errConsecutive'); const v=n?Number(n.textContent):NaN; return !Number.isFinite(v)||v===0; }

    function updateMachineMascot(state, cls) {
      const mascot = $('machineMascot');
      if (!mascot) return;

      let mode = 'off';
      let label = state || 'Выключена';

      if (!machineOn) {
        mode = 'off';
        label = 'Выключена';
      } else if (!pressureSensorOk()) {
        mode = 'error';
        label = 'Ошибка датчика';
      } else if (waterAvailable === false) {
        mode = 'warning';
        label = 'Мало воды';
      } else if (shotRunning) {
        mode = 'shot';
        label = 'Пролив';
      } else if (hotWaterOn) {
        mode = 'water';
        label = 'Подача воды';
      } else if (steamModeOn) {
        mode = 'steam';
      } else if (cls === 'ready') {
        mode = 'ready';
      } else {
        mode = 'heating';
      }

      mascot.className = 'machine-mascot state-' + mode;
      const localized = tr(label);
      const waterWarning = waterAvailable === false && localized !== tr('Мало воды');
      const title = waterWarning ? localized + ' · ' + tr('Мало воды') : localized;
      const aria = tr('Состояние кофемашины') + ': ' + title;
      mascot.title = title;
      mascot.setAttribute('aria-label', aria);
      mascot.dataset.state = mode;
    }

    function updateHomeDashboard() {
      const temp=Number(values.temp), brew=Number(values.tempTarget), steam=Number(values.steamTargetLive);
      const target=steamModeOn&&Number.isFinite(steam)?steam:brew;
      if($('temp')) $('temp').textContent=Number.isFinite(temp)?fmt(temp):'—';
      if($('tempTarget')) $('tempTarget').textContent=Number.isFinite(target)?fmt(target):'—';
      if($('homeThermalTemp')) $('homeThermalTemp').textContent=Number.isFinite(temp)?fmt(temp):'—';
      if($('homeThermalTarget')) $('homeThermalTarget').textContent=Number.isFinite(target)?fmt(target):'—';
      const livePressureTarget=Number(values.target);
      const profilePressureTarget=Number(profileCfg.mainPressure);
      const pressureTarget=shotRunning&&Number.isFinite(livePressureTarget)?livePressureTarget:profilePressureTarget;
      if($('homePressureTarget')) $('homePressureTarget').textContent=Number.isFinite(pressureTarget)?fmt(pressureTarget,1):'—';
      const targetWeight=Number($('targetWeight')?.value);
      if($('homeWeightTarget')) $('homeWeightTarget').textContent=Number.isFinite(targetWeight)?fmt(targetWeight,targetWeight%1?1:0):'—';
      if($('homeWeightCard')) $('homeWeightCard').classList.toggle('live',shotRunning);
      const heat=Number(values.heat);
      if($('homeHeat')) $('homeHeat').textContent=Number.isFinite(heat)?fmt(heat,0):'—';
      if($('homeHeatFill')) $('homeHeatFill').style.width=(Number.isFinite(heat)?Math.max(0,Math.min(100,heat)):0)+'%';
      const homeHeatTrack=document.querySelector('.home-temperature-heat-track');
      if(homeHeatTrack){
        const heatPct=Number.isFinite(heat)?Math.max(0,Math.min(100,heat)):0;
        homeHeatTrack.style.setProperty('--heat-pct',heatPct+'%');
      }
      const tempPanel=$('homeTemperatureState'), pidState=$('homePidState'), tempDelta=$('homeTempDelta');
      if(tempPanel){
        tempPanel.classList.remove('heating','ready','error');
        let pidLabel='Выключен';
        if(machineOn){
          if(!pressureSensorOk()){tempPanel.classList.add('error');pidLabel='Ошибка';}
          else if(Number.isFinite(temp)&&Number.isFinite(target)&&Math.abs(target-temp)<=0.5){tempPanel.classList.add('ready');pidLabel='Удержание';}
          else{tempPanel.classList.add('heating');pidLabel=Number.isFinite(heat)&&heat>1?'Нагрев':'PID активен';}
        }
        if(pidState) pidState.textContent=tr(pidLabel);
      }
      if(tempDelta){
        if(Number.isFinite(temp)&&Number.isFinite(target)){
          const delta=target-temp;
          tempDelta.textContent=Math.abs(delta)<=0.05?tr('Температура на цели'):(delta>0?tr('До цели')+' '+fmt(delta,1)+' °C':tr('Выше цели')+' '+fmt(Math.abs(delta),1)+' °C');
        }else tempDelta.textContent=tr('Ожидание данных');
      }
      const autoOffNode=$('machineAutoOff'), autoOffText=$('machineAutoOffText');
      if(autoOffNode&&autoOffText){
        const remainingAutoOff=Number(values.autoOffRemaining);
        autoOffNode.hidden=!machineOn;
        autoOffNode.classList.remove('warn');
        autoOffNode.classList.toggle('shot-phase-countdown',shotRunning);
        if(machineOn&&!shotRunning){
          if(Number.isFinite(remainingAutoOff)&&remainingAutoOff>0){
            const minutes=Math.ceil(remainingAutoOff);
            autoOffText.textContent=tr('через')+' '+minutes+' '+tr('мин');
            autoOffNode.classList.toggle('warn',minutes<=5);
          }else if(Number.isFinite(remainingAutoOff)){
            autoOffText.textContent=tr('выключено');
          }else{
            autoOffText.textContent='—';
          }
        }
      }
      let state='Выключена', cls='off';
      if(machineOn){
        if(!pressureSensorOk()){state='Ошибка датчика';cls='error';}
        else if(shotRunning){state='Пролив';cls='busy';}
        else if(hotWaterOn){state='Подача воды';cls='busy';}
        else if(steamModeOn){const ready=Number.isFinite(temp)&&Number.isFinite(target)&&temp>=target-2;state=ready?'Пар готов':'Нагрев пара';cls=ready?'ready':'busy';}
        else {const ready=Number.isFinite(temp)&&Number.isFinite(target)&&temp>=target-1;state=ready?'Готова':'Нагрев';cls=ready?'ready':'busy';}
      }
      if($('machineState')){$('machineState').textContent=tr(state);$('machineState').className='machine-state '+cls;}
      updateMachineMascot(state, cls);
      const waterBadge=$('machineWaterStatus'), waterText=$('machineWaterText'), waterIcon=$('machineWaterIcon'), waterAlert=$('machineMascotWaterAlert');
      if(waterBadge&&waterText){
        waterBadge.classList.remove('ok','warn','unknown');
        if(waterAvailable==null){
          waterBadge.classList.add('unknown');
          waterText.textContent=tr('Нет данных');
          if(waterIcon) waterIcon.textContent='?';
          if(waterAlert) waterAlert.textContent='?';
        }else if(waterAvailable){
          waterBadge.classList.add('ok');
          waterText.textContent=tr('Вода есть');
          if(waterIcon) waterIcon.textContent='✓';
          if(waterAlert) waterAlert.textContent='';
        }else{
          waterBadge.classList.add('warn');
          waterText.textContent=tr('Мало воды');
          if(waterIcon) waterIcon.textContent='!';
          if(waterAlert) waterAlert.textContent='!';
        }

        const waterLabel =
          waterText.textContent ||
          tr('Состояние бака воды');
        waterBadge.title = waterLabel;
        waterBadge.setAttribute(
          'aria-label',
          waterLabel
        );
      }
      const shots=Number(values.backflushShots), limit=Number(values.backflushReminderLive);
      const backflushDue=
        Number.isFinite(limit)&&
        limit>0&&
        Number.isFinite(shots)&&
        shots>=limit;

      if($('backflushMain')) {
        if(Number.isFinite(shots)&&Number.isFinite(limit)&&limit>0) {
          $('backflushMain').textContent=
            backflushDue
              ?tr('Пора')
              :tr('ОК');
        } else if(Number.isFinite(shots)) {
          $('backflushMain').textContent=
            tr('Счётчик');
        } else {
          $('backflushMain').textContent='—';
        }
      }

      if($('backflushCount')) {
        $('backflushCount').textContent=
          Number.isFinite(shots)
            ?fmt(shots,0)
            :'—';
      }

      if($('backflushLimit')) {
        $('backflushLimit').textContent=
          Number.isFinite(limit)&&limit>0
            ?fmt(limit,0)
            :'—';
      }

      if($('backflushCard')) {
        $('backflushCard').classList.toggle(
          'due',
          backflushDue
        );
      }
      if($('homeMonthlyShots')){
        const monthlyShots=Number(values.monthlyShots);
        $('homeMonthlyShots').textContent=Number.isFinite(monthlyShots)?fmt(monthlyShots,0):'—';
      }
      if($('homeMonthlyWeight')){
        const monthlyWeight=Number(values.monthlyDrinkWeight);
        $('homeMonthlyWeight').textContent=Number.isFinite(monthlyWeight)?fmt(monthlyWeight,0):'—';
      }
      const backflushRemaining=$('homeBackflushRemaining');
      const backflushStat=$('homeBackflushStat');
      const backflushCaption=$('homeBackflushStatCaption');
      if(backflushRemaining&&backflushStat&&backflushCaption){
        if(Number.isFinite(limit)&&limit>0&&Number.isFinite(shots)){
          backflushRemaining.textContent=backflushDue?tr('Пора'):fmt(Math.max(0,limit-shots),0);
          backflushCaption.textContent=backflushDue?tr('нужна промывка'):tr('проливов');
          backflushStat.classList.toggle('due',backflushDue);
        }else{
          backflushRemaining.textContent='—';
          backflushCaption.textContent=tr('проливов');
          backflushStat.classList.remove('due');
        }
      }
      if($('homeBackflushHint')){
        if(Number.isFinite(limit)&&limit>0&&Number.isFinite(shots)) $('homeBackflushHint').textContent=shots>=limit?tr('Требуется промывка'):tr('До промывки')+' '+fmt(Math.max(0,limit-shots),0);
        else $('homeBackflushHint').textContent=tr('Счётчик проливов');
      }

      const pressureBackflush = $('homePressureBackflush');
      const pressureBackflushText = $('homePressureBackflushText');
      if (pressureBackflush && pressureBackflushText) {
        pressureBackflush.classList.toggle('due', backflushDue);
        if (
          Number.isFinite(shots) &&
          Number.isFinite(limit) &&
          limit > 0
        ) {
          const remaining = Math.max(0, limit - shots);
          pressureBackflushText.textContent = backflushDue
            ? tr('Требуется промывка')
            : tr('До промывки') +
              ' · ' +
              fmt(remaining, 0) +
              ' ' +
              tr('проливов');
        } else {
          pressureBackflushText.textContent =
            tr('Счётчик проливов');
        }
      }

      requestAnimationFrame(() => {
        drawHomePressureGauge();
        drawHomeTemperatureMini();
      });
    }
    function updateHomeProfileRail(options = availableProfileOptions, value = currentProfileName) {
      const list = Array.isArray(options) && options.length
        ? options
        : availableProfileOptions;
      const dirty = !!(
        mainCurveDirty ||
        recipeDirty ||
        profilePhaseEditMode ||
        mainCurveEditing
      );
      const current = dirty ? 'Custom' : (value || currentProfileName);
      customProfileAvailable = customDraftIsAvailable();
      const railCurrent = $('homeProfileRailCurrent');

      if (railCurrent) {
        railCurrent.textContent = current ? profileLabel(current) : '—';
      }

      const root = $('homeProfileButtons');
      if (!root) return;

      const existing = [...root.querySelectorAll('button')]
        .map(button => button.dataset.profile);

      if (
        existing.length !== list.length ||
        existing.some((profile,index) => profile !== list[index])
      ) {
        root.innerHTML = '';

        list.forEach(option => {
          const button = document.createElement('button');
          button.type = 'button';
          button.dataset.profile = option;
          button.textContent = profileLabel(option);
          button.onclick = () => applyHomeProfile({ value:option });
          root.appendChild(button);
        });
      }

      const disabled =
        shotRunning ||
        recipeApplying ||
        homeProfileApplying ||
        !!homeQuickField ||
        !!mainCurveDragKey;

      root.querySelectorAll('button').forEach(button => {
        const profile = button.dataset.profile;
        const unavailableCustom =
          profile === 'Custom' &&
          !customProfileAvailable;
        button.classList.remove('custom-unavailable');
        button.textContent = profileLabel(profile);
        button.title = unavailableCustom
          ? tr('Измените любой профиль и нажмите «Применить»')
          : profileLabel(profile);
        button.classList.toggle(
          'active',
          profile === current && !unavailableCustom
        );
        button.disabled = disabled || unavailableCustom;
      });
    }


    function userProfileId() {
      if (globalThis.crypto && typeof globalThis.crypto.randomUUID === 'function') return globalThis.crypto.randomUUID();
      return 'profile-' + Date.now().toString(36) + '-' + Math.random().toString(36).slice(2,9);
    }

    function sanitizeUserProfileConfig(config) {
      const clean = {};
      recipeFieldIds.forEach(id => {
        const value = Number(config?.[id]);
        if (Number.isFinite(value)) clean[id] = value;
      });
      const spec = String(config?.phases || '').trim();
      clean.phases = parseAdvancedPhaseSpec(spec) ? spec : '';
      return clean;
    }

    async function loadUserProfiles(options = {}) {
      try {
        const response = await fetch('/profiles/index.json', { cache:'no-store' });
        if (!response.ok) throw new Error('profiles_http_' + response.status);
        const payload = await response.json();
        userProfiles = Array.isArray(payload?.profiles)
          ? payload.profiles
              .filter(item => item && typeof item.id === 'string' && typeof item.name === 'string' && item.config)
              .map(item => ({
                id:item.id,
                name:item.name.trim().slice(0,36),
                config:sanitizeUserProfileConfig(item.config),
                createdAt:Number(item.createdAt) || 0,
                updatedAt:Number(item.updatedAt) || Number(item.createdAt) || 0
              }))
              .filter(item => item.name)
          : [];
        userProfilesLoadError = false;
        renderUserProfiles();
        return true;
      } catch (_) {
        userProfilesLoadError = true;
        renderUserProfiles();
        if (!options.silent) toast(tr('Не удалось загрузить профили с кофемашины'), true);
        return false;
      }
    }

    function userProfileForm(item) {
      const body = new URLSearchParams({
        id:String(item.id),
        name:String(item.name),
        createdAt:String(Math.trunc(Number(item.createdAt) || Date.now())),
        updatedAt:String(Math.trunc(Number(item.updatedAt) || Date.now()))
      });
      recipeFieldIds.forEach(id => body.set(id, String(Number(item.config?.[id]))));
      body.set('phases', String(item.config?.phases || ''));
      return body;
    }

    async function storeUserProfile(item) {
      const response = await fetch('/profiles/save', {
        method:'POST',
        headers:{ 'Content-Type':'application/x-www-form-urlencoded;charset=UTF-8' },
        body:userProfileForm(item)
      });
      if (!response.ok) {
        let error = '';
        try { error = String((await response.json())?.error || ''); } catch (_) {}
        if (response.status === 409 || error === 'profile_limit') {
          throw new Error('profile_limit');
        }
        throw new Error(error || ('profiles_http_' + response.status));
      }
      return true;
    }

    function userProfileMeta(config) {
      const temperature = Number(config?.brewTarget);
      const weight = Number(config?.targetWeight);
      const shot = Number(config?.shotSeconds);
      const pieces = [];
      if (Number.isFinite(temperature)) pieces.push(fmt(temperature, temperature % 1 ? 1 : 0) + ' °C');
      if (Number.isFinite(weight)) pieces.push(fmt(weight, weight % 1 ? 1 : 0) + ' ' + tr('г'));
      if (Number.isFinite(shot)) pieces.push(fmt(shot, shot % 1 ? 1 : 0) + ' ' + tr('с'));
      return pieces.join(' · ') || tr('Пользовательские настройки');
    }

    function userProfileDate(item) {
      const createdAt = Number(item?.createdAt);
      const updatedAt = Number(item?.updatedAt);
      const timestamp = Number.isFinite(updatedAt) ? updatedAt : createdAt;
      if (!Number.isFinite(timestamp)) return '';

      const locale = currentLanguage === 'en' ? 'en-GB' : 'ru-RU';
      const date = new Intl.DateTimeFormat(locale, {
        day:'2-digit',
        month:'2-digit',
        year:'2-digit',
        hour:'2-digit',
        minute:'2-digit'
      }).format(new Date(timestamp));
      const changed =
        Number.isFinite(createdAt) &&
        Number.isFinite(updatedAt) &&
        updatedAt - createdAt > 1000;
      return tr(changed ? 'Обновлён' : 'Сохранён') + ': ' + date;
    }

    function renderUserProfiles() {
      const profileSelect = $('homeProfileSelect');
      if (profileSelect) {
        [...profileSelect.options].forEach(option => {
          option.disabled = option.value === 'Custom' && !customProfileAvailable;
        });
      }
      updateHomeProfileRail(availableProfileOptions,currentProfileName);

      const root = $('userProfileList');
      const count = $('userProfileCount');
      if (count) count.textContent = String(userProfiles.length);
      if (!root) return;
      root.innerHTML = '';

      if (!userProfiles.length) {
        const empty = document.createElement('div');
        empty.className = 'user-profile-empty';
        empty.textContent = tr(userProfilesLoadError
          ? 'Хранилище профилей временно недоступно'
          : 'Сохранённых профилей пока нет');
        root.appendChild(empty);
        return;
      }

      userProfiles
        .slice()
        .sort((a,b) => b.updatedAt - a.updatedAt)
        .forEach(item => {
          const row = document.createElement('div');
          row.className = 'user-profile-row';
          row.classList.toggle('active', item.id === activeUserProfileId);

          const applyButton = document.createElement('button');
          applyButton.type = 'button';
          applyButton.className = 'user-profile-apply';
          applyButton.disabled = userProfileApplying || userProfileStorageBusy || shotRunning;
          applyButton.onclick = () => applyUserProfile(item.id);

          const name = document.createElement('b');
          name.textContent = item.name;
          const meta = document.createElement('small');
          meta.textContent = userProfileMeta(item.config);
          const date = document.createElement('small');
          date.className = 'user-profile-date';
          date.textContent = userProfileDate(item);
          applyButton.append(name, meta, date);

          const actions = document.createElement('div');
          actions.className = 'user-profile-actions';

          const rename = document.createElement('button');
          rename.type = 'button';
          rename.textContent = '✎';
          rename.title = tr('Переименовать профиль');
          rename.setAttribute('aria-label', tr('Переименовать профиль'));
          rename.disabled = userProfileApplying || userProfileStorageBusy || shotRunning;
          rename.onclick = () => renameUserProfile(item.id);

          const remove = document.createElement('button');
          remove.type = 'button';
          remove.className = 'user-profile-delete';
          remove.textContent = '🗑';
          remove.title = tr('Удалить профиль');
          remove.setAttribute('aria-label', tr('Удалить профиль'));
          remove.disabled = userProfileApplying || userProfileStorageBusy || shotRunning;
          remove.onclick = () => deleteUserProfile(item.id);

          actions.append(rename, remove);
          row.append(applyButton, actions);
          root.appendChild(row);
        });
    }

    async function saveCurrentUserProfile() {
      if (userProfileApplying || userProfileStorageBusy || shotRunning) return;

      const hasDraft = !!(mainCurveDirty || recipeDirty || activeCustomPhaseSpec);
      if (!hasDraft) {
        toast(tr('Сначала измените профиль'), true);
        return;
      }

      const input = $('userProfileName');
      const name = String(input?.value || '').trim().slice(0,36);

      if (!name) {
        if (input) {
          input.classList.remove('attention');
          void input.offsetWidth;
          input.classList.add('attention');
          input.focus();
        }
        toast(tr('Введите название профиля'), true);
        return;
      }

      const config = sanitizeUserProfileConfig(recipeConfigFromInputs());
      let item = userProfiles.find(profile =>
        profile.name.toLocaleLowerCase() === name.toLocaleLowerCase()
      );
      const existed = !!item;
      const now = Date.now();
      const previousConfig = existed ? { ...item.config } : null;
      const previousUpdatedAt = existed ? item.updatedAt : 0;

      if (item) {
        item.config = config;
        item.updatedAt = now;
      } else {
        item = {
          id:userProfileId(),
          name,
          config,
          createdAt:now,
          updatedAt:now
        };
        userProfiles.push(item);
      }

      userProfileStorageBusy = true;
      renderUserProfiles();
      try {
        await storeUserProfile(item);
      } catch (error) {
        userProfileStorageBusy = false;
        if (!existed) {
          userProfiles = userProfiles.filter(profile => profile.id !== item.id);
        } else {
          item.config = previousConfig;
          item.updatedAt = previousUpdatedAt;
        }
        renderUserProfiles();
        toast(tr(error?.message === 'profile_limit'
          ? 'На кофемашине уже сохранено 30 профилей'
          : 'Не удалось сохранить профиль на кофемашине'), true);
        return;
      }
      userProfileStorageBusy = false;

      if (input) input.value = '';
      renderUserProfiles();
      await loadUserProfiles({ silent:true });

      const applied = await applyUserProfile(item.id, { silent:true });

      if (applied) {
        toast(tr(existed ? 'Профиль обновлён и применён' : 'Профиль сохранён и применён'));
      } else {
        toast(tr('Профиль сохранён, но не применён'), true);
      }
    }

    async function renameUserProfile(id) {
      if (userProfileApplying || userProfileStorageBusy || shotRunning) return;
      const item = userProfiles.find(profile => profile.id === id);
      if (!item) return;
      const next = prompt(tr('Новое название профиля'), item.name);
      if (next === null) return;
      const name = String(next).trim().slice(0,36);
      if (!name) return;

      const duplicate = userProfiles.find(profile => profile.id !== id && profile.name.toLocaleLowerCase() === name.toLocaleLowerCase());
      if (duplicate) {
        toast(tr('Профиль с таким названием уже существует'), true);
        return;
      }

      const previousName = item.name;
      const previousUpdatedAt = item.updatedAt;
      item.name = name;
      item.updatedAt = Date.now();
      userProfileStorageBusy = true;
      renderUserProfiles();
      try {
        await storeUserProfile(item);
        userProfileStorageBusy = false;
        renderUserProfiles();
        await loadUserProfiles({ silent:true });
        toast(tr('Профиль переименован'));
      } catch (_) {
        userProfileStorageBusy = false;
        item.name = previousName;
        item.updatedAt = previousUpdatedAt;
        renderUserProfiles();
        toast(tr('Не удалось переименовать профиль на кофемашине'), true);
      }
    }

    async function deleteUserProfile(id) {
      if (userProfileApplying || userProfileStorageBusy || shotRunning) return;
      const item = userProfiles.find(profile => profile.id === id);
      if (!item) return;
      const deletingActiveProfile = activeUserProfileId === id;
      if (!confirm(tr('Удалить профиль') + ' «' + item.name + '»?')) return;
      userProfileStorageBusy = true;
      renderUserProfiles();
      try {
        const response = await fetch('/profiles/delete/' + encodeURIComponent(id), { method:'POST' });
        if (!response.ok) throw new Error('profiles_http_' + response.status);
        userProfileStorageBusy = false;
        userProfiles = userProfiles.filter(profile => profile.id !== id);
        if (activeUserProfileId === id) activeUserProfileId = '';
        renderUserProfiles();
        if (deletingActiveProfile && currentProfileName === 'Custom') {
          const fallback = availableProfileOptions.find(option => option !== 'Custom') || 'Classic';
          await applyHomeProfile({ value:fallback });
        }
        toast(tr('Профиль удалён'));
      } catch (_) {
        userProfileStorageBusy = false;
        renderUserProfiles();
        toast(tr('Не удалось удалить профиль с кофемашины'), true);
      }
    }

    async function applyUserProfile(id, options = {}) {
      if (userProfileApplying || shotRunning) return false;

      const item = userProfiles.find(profile => profile.id === id);
      if (!item) return false;

      const silent = !!options.silent;
      const previousActive = activeUserProfileId;
      const previousConfig = recipeConfigFromInputs();
      const previousProfile = currentProfileName;

      userProfileApplying = true;
      activeUserProfileId = id;
      renderUserProfiles();

      applyRecipeConfig(item.config);
      readProfileCfgFromInputs();

      currentProfileName = 'Custom';
      recipeBaseProfile = 'Custom';
      recipeDirty = false;
      chartMode = 'auto';

      syncProfileControls('Custom');
      updateRecipeProfileUi();
      updateRecipeDescription();
      updateShotScenario();
      draw();
      drawRecipePreview();

      try {
        for (const [fieldId, entityName] of Object.entries(nums)) {
          const value = Number(item.config[fieldId]);
          if (Number.isFinite(value)) {
            await post('number', entityName, 'set', { value });
          }
        }

        await applyAdvancedPhaseSpec(String(item.config.phases || ''));
        await post('select', 'Silvia Shot Profile', 'set', { option:'Custom' });

        clearMainCurveDraftState();
        currentProfileName = 'Custom';
        recipeBaseProfile = 'Custom';
        syncProfileControls('Custom');
        captureRecipeBaseline('Custom');
        updateMainCurveEditor();

        if (!silent) toast(tr('Профиль применён') + ': ' + item.name);
        return true;
      } catch (_) {
        activeUserProfileId = previousActive;
        applyRecipeConfig(previousConfig);
        readProfileCfgFromInputs();
        currentProfileName = previousProfile;
        recipeBaseProfile = previousProfile || recipeBaseProfile;
        syncProfileControls(previousProfile);
        updateRecipeProfileUi();
        updateRecipeDescription();
        updateMainCurveEditor();
        draw();
        drawRecipePreview();

        if (!silent) toast(tr('Профиль не применён'), true);
        if (!eventStreamHealthy()) await loadHomeProfileSnapshot(false);

        return false;
      } finally {
        userProfileApplying = false;
        renderUserProfiles();
        updateQuickActions();
      }
    }

    function fillHomeProfileSelect(options = availableProfileOptions, value = currentProfileName) {
      const select = $('homeProfileSelect');
      const list = Array.isArray(options) && options.length ? options : availableProfileOptions;
      customProfileAvailable = customDraftIsAvailable();
      if (select) {
        const existing = [...select.options].map(option => option.value);
        if (existing.length !== list.length || existing.some((item,index) => item !== list[index])) {
          select.innerHTML = '';
          list.forEach(option => select.add(new Option(profileLabel(option), option)));
        } else {
          [...select.options].forEach(option => { option.textContent = profileLabel(option.value); });
        }
        [...select.options].forEach(option => {
          option.disabled = option.value === 'Custom' && !customProfileAvailable;
        });
        if (value && [...select.options].some(option => option.value === value)) select.value = value;
      }
      updateHomeProfileRail(list, value);
    }

    function syncProfileControls(value = currentProfileName) {
      const profile = String(value || '');
      if ($('profile')) $('profile').textContent = profile ? profileLabel(profile) : '—';
      fillHomeProfileSelect(availableProfileOptions, profile);
      const recipeSelect = $('profileSelect');
      if (recipeSelect && [...recipeSelect.options].some(option => option.value === profile)) recipeSelect.value = profile;
      updateShotScenario();
    }

    function currentProfileTargetWeight() {
      const direct = Number($('targetWeight')?.value);
      if (Number.isFinite(direct)) return direct;

      const config = profileDisplayConfig();
      const fallback = Number(config?.targetWeight);
      return Number.isFinite(fallback) ? fallback : NaN;
    }

    function updateShotWeightFooter(config = null) {
      const control = document.querySelector(
        '.target-weight-footer[data-home-scrub="targetWeight"]'
      );
      const label = $('homeSummaryTargetWeightLabel');
      const value = $('homeSummaryTargetWeight');
      if (!control || !label || !value) return;

      const configuredTarget = Number(
        config?.targetWeight
      );
      const target = Number.isFinite(configuredTarget)
        ? configuredTarget
        : currentProfileTargetWeight();

      const resultVisible =
        !shotRunning &&
        Number.isFinite(lastShotDrinkWeight);

      control.classList.toggle(
        'shot-result',
        resultVisible
      );

      if (resultVisible) {
        const resultTarget = Number.isFinite(lastShotTargetWeight)
          ? lastShotTargetWeight
          : target;
        label.textContent =
          tr('Вес') + ' / ' + tr('Цель');
        value.textContent =
          fmt(Math.max(0, lastShotDrinkWeight), 1) +
          (Number.isFinite(resultTarget)
            ? ' / ' +
              fmt(
                resultTarget,
                resultTarget % 1 ? 1 : 0
              )
            : '') +
          ' ' + tr('г');
        control.setAttribute(
          'aria-label',
          tr('Вес') + ' ' +
          fmt(Math.max(0, lastShotDrinkWeight), 1) +
          ' ' + tr('г') +
          (Number.isFinite(resultTarget)
            ? ', ' + tr('Цель') + ' ' +
              fmt(resultTarget, resultTarget % 1 ? 1 : 0) +
              ' ' + tr('г')
            : '')
        );
      } else {
        label.textContent = tr('Целевой вес');
        value.textContent = Number.isFinite(target)
          ? fmt(target, target % 1 ? 1 : 0) + ' ' + tr('г')
          : '—';
        control.setAttribute(
          'aria-label',
          tr('Изменить целевой вес')
        );
      }
    }

    function observeShotWeight(value) {
      const weight = Number(value);
      if (!Number.isFinite(weight)) return;

      const normalized = Math.max(0, weight);
      const now = performance.now();

      if (shotRunning) {
        if (
          !Number.isFinite(shotPeakWeight) ||
          normalized > shotPeakWeight
        ) {
          shotPeakWeight = normalized;
        }
        return;
      }

      if (
        now <= postShotWeightCaptureUntil &&
        (
          !Number.isFinite(lastShotDrinkWeight) ||
          normalized > lastShotDrinkWeight
        )
      ) {
        lastShotDrinkWeight = normalized;
        updateShotWeightFooter();
      }
    }

    function updateShotScenario() {
      const profile = currentProfileName || $('profileSelect')?.value || 'Custom';
      const config =
        profileDisplayConfig();
      const prePump = Math.max(0, Number(config.prePump) || 0);
      const prePause = Math.max(0, Number(config.prePause) || 0);
      const shotSeconds = Math.max(0, Number(config.shotSeconds) || 0);
      const phases = displayedAdvancedPhases(profile, config);
      const total = phases ? phaseDraftTotalSeconds(phases) : prePump + prePause + shotSeconds;

      const setStageChip = (field, valueId, label, value, summaryOnly) => {
        const node = $(valueId);
        const chip = node?.closest('button');
        const small = chip?.querySelector('small');
        if (small) small.textContent = tr(label);
        if (node) node.textContent = value;
        if (!chip) return;
        chip.classList.toggle('phase-summary-only', summaryOnly);
        if (summaryOnly) delete chip.dataset.homeScrub;
        else chip.dataset.homeScrub = field;
      };

      if ($('homeSummaryBrewTarget')) $('homeSummaryBrewTarget').textContent = fmt(config.brewTarget, config.brewTarget % 1 ? 1 : 0) + ' °C';
      if (phases) {
        const brewPhases = phases.filter(item => item.type !== 'pause');
        const pausePhases = phases.filter(item => item.type === 'pause');
        const brewSeconds = phaseDraftTotalSeconds(brewPhases);
        const pauseSeconds = phaseDraftTotalSeconds(pausePhases);
        const countAndTime = (count, seconds) => count + ' · ' + fmt(seconds, seconds % 1 ? 1 : 0) + ' ' + tr('с');
        setStageChip('prePump', 'homeSummaryPrePump', 'Подачи', countAndTime(brewPhases.length, brewSeconds), true);
        setStageChip('prePause', 'homeSummaryPrePause', 'Паузы', countAndTime(pausePhases.length, pauseSeconds), true);
        setStageChip('shotSeconds', 'homeSummaryShotSeconds', 'Этапы', phases.length + '', true);
      } else {
        setStageChip('prePump', 'homeSummaryPrePump', 'Предсмачивание', fmt(prePump, prePump % 1 ? 1 : 0) + ' ' + tr('с'), false);
        setStageChip('prePause', 'homeSummaryPrePause', 'Пауза', fmt(prePause, prePause % 1 ? 1 : 0) + ' ' + tr('с'), false);
        setStageChip('shotSeconds', 'homeSummaryShotSeconds', 'Пролив', fmt(shotSeconds, shotSeconds % 1 ? 1 : 0) + ' ' + tr('с'), false);
      }
      renderPhaseStripView();
      updateShotWeightFooter(config);

      const badge = $('homeProfileEditBadge');
      if (badge) {
        badge.textContent = recipeDirty ? profileLabel('Custom') + '*' : profileLabel(profile);
        badge.classList.toggle('dirty', recipeDirty);
      }
      syncHomeProfileEditor();
    }

    function mainCurveCanEdit() {
      return !shotRunning &&
        chartMode !== 'archive' &&
        !recipeApplying &&
        !homeProfileApplying &&
        !homeQuickField;
    }

    function prepareProfileForCurveDraft() {
      if (mainCurveDirty || recipeDirty) return;

      const profile =
        currentProfileName ||
        $('profileSelect')?.value ||
        'Custom';

      /*
       * profileCfg is the model currently visible on the canvas.
       * After a fresh page load the hidden recipe inputs may still contain
       * their HTML defaults. Copy the complete visible model before dragging.
       */
      const visible = {
        prePump:Math.max(0, Number(profileCfg.prePump) || 0),
        prePause:Math.max(0, Number(profileCfg.prePause) || 0),
        prePower:Math.max(0, Number(profileCfg.prePower) || 0),
        mainPressure:Math.max(0, Number(profileCfg.mainPressure) || 0),
        endPressure:Math.max(0, Number(profileCfg.endPressure) || 0),
        shotSeconds:Math.max(0, Number(profileCfg.shotSeconds) || 0),
        softInfusionTime:Math.max(
          0,
          Number(profileCfg.softInfusionTime) || 0
        ),
        softInfusionStartPower:Math.max(
          0,
          Number(profileCfg.softInfusionStartPower) || 0
        )
      };

      const builtInRamp = builtInProfileRampSeconds(
        profile,
        visible.shotSeconds
      );
      if (Number.isFinite(builtInRamp)) {
        visible.softInfusionTime = builtInRamp;
      }
      if (profile === 'Classic') {
        visible.prePower = visible.mainPressure;
        visible.endPressure = visible.mainPressure;
      }

      [
        'prePump',
        'prePause',
        'prePower',
        'mainPressure',
        'endPressure',
        'shotSeconds',
        'softInfusionTime',
        'softInfusionStartPower'
      ].forEach(id => {
        const input = $(id);
        const value = Number(visible[id]);

        if (!input || !Number.isFinite(value)) return;
        input.value = value.toFixed(1);
      });

      /*
       * Read once after synchronization. From here the canvas, drag snapshot
       * and all hidden inputs describe exactly the same profile.
       */
      readProfileCfgFromInputs();
    }

    function beginMainCurveEditSession() {
      if (!mainCurveCanEdit()) {
        if (shotRunning) {
          toast(tr('Редактирование недоступно во время пролива'), true);
        }
        return false;
      }

      prepareProfileForCurveDraft();

      if (!mainCurveBaselineCfg) {
        mainCurveBaselineCfg = recipeConfigFromInputs();
        mainCurveBaselineDirty = recipeDirty;
      }

      chartMode = 'auto';
      mainCurveEditing = true;
      chartHoverIndex = -1;
      hideChartTooltip();

      /*
       * Rebuild the control geometry immediately. The clicked marker keeps
       * the same visual position, but subsequent pointer movement now uses
       * the normalized editable values.
       */
      requestAnimationFrame(draw);
      return true;
    }

    function clearMainCurveDraftState() {
      mainCurveEditing = false;
      mainCurveDirty = false;
      mainCurveBaselineCfg = null;
      mainCurveBaselineDirty = false;
      mainCurveDragKey = '';
      mainCurveHoverKey = '';
      mainCurveDragPointerId = null;
      mainCurveDragStartConfig = null;
      mainCurveClassicLinkedDrag = false;
      chartHoverIndex = -1;
      hideChartTooltip();
    }

    function updateMainCurveEditor() {
      ensureAdvancedPhaseEditor();
      renderAdvancedPhaseEditor();
      const card = $('shotChartCard');
      const profileHint = $('userProfileHint');
      const profileNameInput = $('userProfileName');
      const profileSaveButton = $('userProfileSaveButton');
      const hasDraft = !!(mainCurveDirty || recipeDirty || (currentProfileName === 'Custom' && activeCustomPhaseSpec));
      const saveBusy = userProfileApplying || userProfileStorageBusy || shotRunning;
if (card) {
        card.classList.toggle('curve-ready', mainCurveCanEdit());
        card.classList.toggle('curve-editing', mainCurveEditing || mainCurveDirty);
        card.classList.toggle('curve-dirty', mainCurveDirty);
      }

      if (profileHint) {
        profileHint.textContent = tr(
          hasDraft
            ? 'Есть изменения — введите название и нажмите +'
            : 'Хранятся на кофемашине'
        );
        profileHint.classList.toggle('pending', hasDraft);
      }

      if (profileNameInput) {
        profileNameInput.classList.toggle('pending', hasDraft);
      }

      if (profileSaveButton) {
        profileSaveButton.disabled = !hasDraft || saveBusy;
        profileSaveButton.classList.toggle('pending', hasDraft && !saveBusy);
        profileSaveButton.setAttribute(
          'aria-disabled',
          profileSaveButton.disabled ? 'true' : 'false'
        );
      }

      syncHomeProfileEditor();
      updateShotScenario();
      updateQuickActions();
      updateChartControls();

      requestAnimationFrame(() => {
        draw();
        drawTemp();
      });
    }

    function toggleMainCurveEditor() {
      if (mainCurveDirty) cancelMainCurveEditor();
    }

    function cancelMainCurveEditor(showMessage = true) {
      if (mainCurveBaselineCfg) applyRecipeConfig(mainCurveBaselineCfg);
      recipeDirty = mainCurveBaselineDirty;
      mainCurveEditing = false;
      mainCurveDirty = false;
      mainCurveBaselineCfg = null;
      mainCurveBaselineDirty = false;
      mainCurveDragKey = '';
      mainCurveHoverKey = '';
      mainCurveDragPointerId = null;
      mainCurveDragStartConfig = null;
      mainCurveClassicLinkedDrag = false;
      updateRecipeProfileUi();
      updateRecipeDescription();
      updateMainCurveEditor();
      drawRecipePreview();
      updateShotScenario();
      if (showMessage) toast(tr('Изменения профиля отменены'));
    }

    async function saveMainCurve() {
      if (!mainCurveDirty) return;
      const button = $('mainCurveSaveButton');
      if (button) button.disabled = true;
      const saved = await saveSettings();
      if (!saved) {
        updateMainCurveEditor();
        return;
      }
      mainCurveBaselineCfg = null;
      mainCurveBaselineDirty = false;
      mainCurveDirty = false;
      mainCurveEditing = false;
      mainCurveDragKey = '';
      mainCurveHoverKey = '';
      mainCurveDragStartConfig = null;
      mainCurveClassicLinkedDrag = false;
      updateMainCurveEditor();
      updateShotScenario();
      toast(tr('Профиль сохранён как Custom'));
    }

    function toggleChartExpanded(force) {
      const card = $('shotChartCard');
      const button = $('chartExpandButton');
      if (!card) return;
      const next = typeof force === 'boolean' ? force : !card.classList.contains('expanded');
      card.classList.toggle('expanded', next);
      document.body.classList.toggle('chart-expanded', next);
      if (button) {
        button.textContent = next ? '×' : '⤢';
        button.title = tr(next ? 'Уменьшить график' : 'Увеличить график');
        button.setAttribute('aria-label', button.title);
        button.setAttribute('aria-pressed', next ? 'true' : 'false');
      }
      requestAnimationFrame(() => { draw(); });
    }


    function captureShotPhaseConfig() {
      const config =
        typeof profileDisplayConfig === 'function'
          ? profileDisplayConfig()
          : profileCfg;
      const phases = displayedAdvancedPhases(currentProfileName,config);
      return {
        phases:cloneAdvancedPhases(phases) || [],
        prePump:Math.max(0,Number(config?.prePump)||0),
        prePause:Math.max(0,Number(config?.prePause)||0),
        shotSeconds:Math.max(0,Number(config?.shotSeconds)||0)
      };
    }

    function shotPhaseAtElapsed(elapsedSeconds) {
      const config = shotPhaseConfig || captureShotPhaseConfig();
      const phaseList = Array.isArray(config?.phases) && config.phases.length
        ? config.phases.map((phase,index,all) => ({
            label:phaseDisplayName(phase,index,all),
            duration:Math.max(0,phaseNumber(phase.duration))
          }))
        : [
            {label:'Предсмачивание',duration:config.prePump},
            {label:'Пауза',duration:config.prePause},
            {label:'Пролив',duration:config.shotSeconds}
          ].filter(phase => phase.duration > .001);

      if (!phaseList.length) {
        return {label:'Пролив',elapsed:Math.max(0,elapsedSeconds),duration:0};
      }

      let phaseStart = 0;
      for (let index=0;index<phaseList.length;index++) {
        const phase = phaseList[index];
        const phaseEnd = phaseStart + phase.duration;
        const last = index === phaseList.length-1;
        if (elapsedSeconds < phaseEnd || last) {
          return {
            label:phase.label,
            elapsed:Math.max(0,elapsedSeconds-phaseStart),
            duration:phase.duration
          };
        }
        phaseStart = phaseEnd;
      }
      return phaseList[phaseList.length-1];
    }

    function updateShotPhaseStatus() {
      if (!shotRunning) return;

      const status = $('shotStatus');
      const actionState =
        $('shotActionState');
      const autoOffNode =
        $('machineAutoOff');
      const autoOffText =
        $('machineAutoOffText');
      const elapsed =
        Math.max(
          0,
          (
            performance.now() -
            liveStart
          ) / 1000
        );
      const phase =
        shotPhaseAtElapsed(elapsed);
      const shownElapsed =
        Math.max(
          0,
          Math.floor(
            phase.elapsed
          )
        );
      const shownDuration =
        Math.max(
          0,
          Math.ceil(
            phase.duration
          )
        );

      const phaseLabel =
        tr(phase.label);
      const phaseTimer =
        shownElapsed +
        ' / ' +
        shownDuration +
        ' ' +
        tr('с');
      const text =
        phaseLabel +
        ' · ' +
        phaseTimer;

      if (status) {
        const labelNode =
          document.createElement('span');

        labelNode.className =
          'shot-phase-label';
        labelNode.textContent =
          phaseLabel;

        status.replaceChildren(
          labelNode
        );
        status.classList.add('run');
        status.dataset.phaseLive = '1';
        status.setAttribute(
          'aria-label',
          text
        );
      }

      if (autoOffNode && autoOffText) {
        autoOffNode.hidden = false;
        autoOffNode.classList.remove('warn');
        autoOffNode.classList.add(
          'shot-phase-countdown'
        );
        autoOffText.textContent =
          phaseTimer;
      }

      if (actionState) {
        actionState.textContent = text;
      }
    }

    function restoreShotStatusAfterRun() {
      const status = $('shotStatus');

      if (!status) return;

      const raw = String(
        status.dataset.rawValue ||
        'Ожидание'
      );
      const staleRunStatus =
        /пролив|shot/i.test(raw);

      status.dataset.phaseLive = '0';
      status.classList.remove('run');
      status.removeAttribute('aria-label');
      status.textContent =
        localizeDynamicText(
          staleRunStatus
            ? 'Ожидание'
            : raw
        );

      const autoOffNode =
        $('machineAutoOff');
      if (autoOffNode) {
        autoOffNode.classList.remove(
          'shot-phase-countdown'
        );
      }
    }


    function toggleShotLineQuickSettings(force) {
      const panel = $('shotLineQuickSettings');
      const button = $('shotLineSettingsButton');
      const board = $('shotProfileBoard');
      const profileCard = $('profileLibraryCard');

      if (!panel) return;

      const open =
        typeof force === 'boolean'
          ? force
          : panel.hidden;

      if (open) {
        if (
          board &&
          profileCard &&
          window.matchMedia('(min-width:981px)').matches
        ) {
          const profileHeight =
            profileCard.getBoundingClientRect().height;

          if (profileHeight > 0) {
            board.style.setProperty(
              '--line-settings-profile-height',
              profileHeight + 'px'
            );
          }
        }

        shotLineQuickBaseline =
          cloneShotLineSettings();
        syncShotLineControls();
      } else if (shotLineQuickBaseline) {
        shotLineSettings =
          cloneShotLineSettings(
            shotLineQuickBaseline
          );
        shotLineQuickBaseline = null;
        syncShotLineControls();
        redrawShotLineSettings();
      }

      panel.hidden = !open;
      board?.classList.toggle(
        'line-settings-open',
        open
      );

      if (!open && board) {
        board.style.removeProperty(
          '--line-settings-profile-height'
        );
      }

      if (button) {
        button.classList.toggle('active', open);
        button.setAttribute(
          'aria-expanded',
          open ? 'true' : 'false'
        );
      }

      requestAnimationFrame(draw);
    }

    function updateQuickActions(){
      const sb=$('shotAction'),wb=$('waterAction'),st=$('steamAction');

      if(sb){
        sb.classList.toggle('danger',shotRunning);
        sb.classList.toggle('active',shotRunning);
        sb.disabled=!machineOn||hotWaterOn;
      }

      if($('shotActionTitle')) {
        $('shotActionTitle').textContent=
          tr(shotRunning?'Стоп':'Пролив');
      }

      if(sb) {
        const shotLabel =
          tr(shotRunning?'Стоп':'Пролив');
        sb.setAttribute(
          'aria-label',
          shotLabel
        );
        sb.title = shotLabel;
      }

      if(wb) {
        wb.setAttribute(
          'aria-label',
          tr('Вода')
        );
        wb.title = tr('Вода');
      }

      if(st) {
        st.setAttribute(
          'aria-label',
          tr('Пар')
        );
        st.title = tr('Пар');
      }

      if($('shotActionState')){
        if(shotRunning){
          $('shotActionState').textContent=
            localizeDynamicText($('shotStatus')?.textContent||'Пролив');
        }else{
          const total=
            Math.max(0,Number(profileCfg.prePump)||0)+
            Math.max(0,Number(profileCfg.prePause)||0)+
            Math.max(0,Number(profileCfg.shotSeconds)||0);
          const base=machineOn
            ?tr('Готов к проливу')
            :tr('Машина выключена');

          $('shotActionState').textContent=
            base+
            (machineOn&&total>0
              ?' · '+fmt(total,total%1?1:0)+' '+tr('с')
              :'');
        }
      }

      if(wb){
        wb.classList.toggle('active',hotWaterOn);
        wb.disabled=!machineOn||shotRunning;
      }
      if($('waterActionState')) {
        $('waterActionState').textContent=
          tr(hotWaterOn?'Включено':'Выключено');
      }

      if(st){
        st.classList.toggle('active',steamModeOn);
        st.disabled=!machineOn||shotRunning||hotWaterOn;
      }
      if($('steamActionState')) {
        $('steamActionState').textContent=
          tr(steamModeOn?'Включено':'Выключено');
      }

      const dragBusy=!!mainCurveDragKey;
      const homeProfile=$('homeProfileSelect');

      if(homeProfile) {
        homeProfile.disabled=
          shotRunning||
          recipeApplying||
          homeProfileApplying||
          dragBusy||
          !!homeQuickField;
      }

      document
        .querySelectorAll('button.shot-profile-chip')
        .forEach(button=>{
          button.disabled=
            shotRunning||
            recipeApplying||
            homeProfileApplying||
            dragBusy;
        });

      const stopByWeight=$('stopByWeight');
      if(stopByWeight) {
        stopByWeight.disabled=
          shotRunning||
          recipeApplying||
          homeProfileApplying||
          dragBusy||
          !!homeQuickField;
      }

      updateStopByWeightControl();
      updateHomeProfileRail(availableProfileOptions,currentProfileName);
    }

    function setShotRunning(on) {
      on = !!on;

      if (on === shotRunning) {
        updatePressureTarget();

        if (on) {
          updateShotPhaseStatus();
        }

        return;
      }

      if (on) {
        if (homeQuickField) cancelHomeQuickEditor(false);
        if (mainCurveEditing) cancelMainCurveEditor(false);

        shotPhaseConfig =
          captureShotPhaseConfig();
        shotPeakWeight = NaN;
        shotTargetWeightAtStart =
          currentProfileTargetWeight();
        lastShotDrinkWeight = NaN;
        lastShotTargetWeight = NaN;
        postShotWeightCaptureUntil = 0;
        shotRunning = true;
        liveSeries = [];
        liveStart = performance.now();
        lastPoint = 0;
        chartMode = 'auto';
        shotCompletedThisSession = false;
        renderShotQuality(null);

        updateShotPhaseStatus();
      } else {
        const completedWithData = shotRunning && liveSeries.length > 1;
        if (shotRunning && liveSeries.length) {
          lastShotSeries =
            liveSeries.map(
              point => ({ ...point })
            );
        }

        const stopWeight = Number(values.weight);
        if (Number.isFinite(stopWeight)) {
          observeShotWeight(stopWeight);
        }

        shotRunning = false;
        shotCompletedThisSession = completedWithData;
        lastShotDrinkWeight = Number.isFinite(shotPeakWeight)
          ? Math.max(0, shotPeakWeight)
          : Number.isFinite(stopWeight)
            ? Math.max(0, stopWeight)
            : NaN;
        lastShotTargetWeight = shotTargetWeightAtStart;
        postShotWeightCaptureUntil =
          performance.now() + 4000;
        shotPhaseConfig = null;
        chartMode = 'auto';
        restoreShotStatusAfterRun();
        setTimeout(async () => {
          const loaded = await refreshLatestShotAnalysis();
          if (!loaded) {
            setTimeout(() => void refreshLatestShotAnalysis(), 900);
          }
          if (homeShotRailMode === 'shots') {
            void loadArchiveList(true);
          }
          if ($('page-lab')?.classList.contains('active')) {
            setTimeout(() => void loadTrainingDataset(true), 900);
          }
        }, 350);
      }

      updatePressureTarget();
      updateShotWeightFooter();
      updateHomeDashboard();
      updateQuickActions();
      updateChartControls();
      draw();
    }


    const xdbDiagnosticIds = [
      'errTotal',
      'errStart',
      'errStatus',
      'errTimeout',
      'errPacket',
      'errRejected',
      'errConsecutive',
      'errLast',
      'pressureDataAge'
    ];

    let xdbLastShotLoadedAt = 0;
    let xdbLastShotLoading = false;

    function diagnosticNumber(id) {
      const node = $(id);
      if (!node) return NaN;

      const value = Number(
        String(node.textContent || '')
          .replace(',', '.')
          .replace(/[^\d.+-]/g, '')
      );

      return Number.isFinite(value)
        ? value
        : NaN;
    }

    function diagnosticText(id) {
      const node = $(id);
      const value = String(
        node?.textContent || ''
      ).trim();

      return value && value !== '—'
        ? value
        : '';
    }

    function setXdbCardLevel(node, level) {
      if (!node) return;

      node.classList.remove(
        'checking',
        'ok',
        'warning',
        'critical'
      );
      node.classList.add(level);
    }

    function updateXdbHumanStatus() {
      const card = $('xdbHumanCard');
      const title = $('xdbHumanTitle');
      const text = $('xdbHumanText');
      const consecutiveNode =
        $('xdbHumanConsecutive');
      const lastErrorNode =
        $('xdbHumanLastError');

      const consecutive =
        diagnosticNumber('errConsecutive');
      const total =
        diagnosticNumber('errTotal');
      const dataAge =
        diagnosticNumber('pressureDataAge');
      const lastError =
        diagnosticText('errLast');
      const pump = Number(values.pump);
      const running = !!shotRunning;

      const known =
        Number.isFinite(consecutive);

      let level = 'checking';
      let heading =
        tr('Проверяем связь XDB401…');
      let explanation =
        tr('Получаем состояние датчика и счётчики обмена.');

      if (
        known &&
        consecutive <= 0 &&
        Number.isFinite(dataAge) &&
        dataAge <= 1
      ) {
        level = 'ok';
        heading =
          tr('Связь стабильна');

        if (
          Number.isFinite(total) &&
          total > 0
        ) {
          explanation =
            tr('Сейчас датчик отвечает без ошибок. Накопленные сбои были раньше; их причина видна в последнем CSV и счётчиках ниже.');
        } else {
          explanation =
            tr('Данные давления свежие, ошибок обмена подряд нет.');
        }
      } else if (
        Number.isFinite(dataAge) &&
        dataAge > 1
      ) {
        level = 'critical';
        heading =
          tr('Нет свежих данных XDB401');
        explanation =
          tr('Последнее значение давления старше 1 секунды. Во время пролива защита SENSOR_FAIL должна обнулить выход помпы.');
      } else if (
        known &&
        consecutive < 4
      ) {
        level = 'warning';
        heading =
          tr('Связь нестабильна');
        explanation =
          tr('Идёт короткая серия ошибок обмена. При четырёх ошибках подряд защита SENSOR_FAIL остановит помпу.');
      } else if (known) {
        level = 'critical';
        heading =
          tr('Связь потеряна');

        if (
          running &&
          Number.isFinite(pump) &&
          pump <= .1
        ) {
          explanation =
            tr('Защита SENSOR_FAIL остановила помпу: контроллер не получает надёжные данные давления.');
        } else {
          explanation =
            tr('Контроллер не получает надёжные данные давления. Pressure-профиль нельзя безопасно продолжать.');
        }
      }

      setXdbCardLevel(card, level);

      if (title) title.textContent = heading;
      if (text) text.textContent = explanation;

      if (consecutiveNode) {
        consecutiveNode.textContent =
          known
            ? fmt(consecutive, 0)
            : '—';
      }

      if (lastErrorNode) {
        lastErrorNode.textContent =
          lastError ||
          tr('Нет текущей ошибки');
      }

      updateXdbSystemSummary();
    }

    function updateXdbSystemSummary() {
      const card = $('xdbSystemSummary');
      const text = $('xdbSystemSummaryText');
      const dominantNode =
        $('xdbSystemDominant');

      const counters = [
        {
          label:tr('запуск измерения'),
          value:diagnosticNumber('errStart')
        },
        {
          label:tr('чтение статуса'),
          value:diagnosticNumber('errStatus')
        },
        {
          label:tr('тайм-аут измерения'),
          value:diagnosticNumber('errTimeout')
        },
        {
          label:tr('чтение пакета'),
          value:diagnosticNumber('errPacket')
        },
        {
          label:tr('отбракованные пакеты'),
          value:diagnosticNumber('errRejected')
        }
      ];

      const total =
        diagnosticNumber('errTotal');
      const consecutive =
        diagnosticNumber('errConsecutive');
      const valid = counters.filter(
        item => Number.isFinite(item.value)
      );
      const dominant = valid
        .slice()
        .sort(
          (a, b) => b.value - a.value
        )[0];

      let level = 'checking';
      let summary =
        tr('Загружаем общие счётчики ошибок с момента запуска контроллера.');

      if (Number.isFinite(total)) {
        if (
          Number.isFinite(consecutive) &&
          consecutive >= 4
        ) {
          level = 'critical';
          summary =
            tr('Сейчас идёт серия ошибок XDB401. Ниже показаны накопительные счётчики с момента запуска ESP32.');
        } else if (total > 0) {
          level = 'warning';
          summary =
            tr('Ошибки XDB401 уже фиксировались. Эти значения накопительные и относятся ко всему времени работы после запуска ESP32.');
        } else {
          level = 'ok';
          summary =
            tr('После запуска ESP32 ошибок обмена с XDB401 пока не зарегистрировано.');
        }
      }

      setXdbCardLevel(card, level);

      if (text) {
        text.textContent = summary;
      }

      if (dominantNode) {
        dominantNode.textContent =
          dominant &&
          dominant.value > 0
            ? (
                tr('Чаще всего') +
                ': ' +
                dominant.label +
                ' · ' +
                fmt(dominant.value, 0)
              )
            : tr('Ошибок нет');
      }
    }

    function parseCsvRows(csvText) {
      const source = String(csvText || '')
        .replace(/^\uFEFF/, '')
        .trim();

      if (!source) return [];

      const firstLine =
        source.split(/\r?\n/, 1)[0] || '';
      const delimiter =
        (firstLine.match(/;/g) || []).length >
        (firstLine.match(/,/g) || []).length
          ? ';'
          : ',';

      const rows = [];
      let row = [];
      let value = '';
      let quoted = false;

      for (
        let index = 0;
        index < source.length;
        index += 1
      ) {
        const char = source[index];

        if (quoted) {
          if (
            char === '"' &&
            source[index + 1] === '"'
          ) {
            value += '"';
            index += 1;
          } else if (char === '"') {
            quoted = false;
          } else {
            value += char;
          }
          continue;
        }

        if (char === '"') {
          quoted = true;
        } else if (char === delimiter) {
          row.push(value);
          value = '';
        } else if (
          char === '\n' ||
          char === '\r'
        ) {
          if (
            char === '\r' &&
            source[index + 1] === '\n'
          ) {
            index += 1;
          }

          row.push(value);
          value = '';

          if (
            row.some(cell =>
              String(cell).trim() !== ''
            )
          ) {
            rows.push(row);
          }

          row = [];
        } else {
          value += char;
        }
      }

      row.push(value);

      if (
        row.some(cell =>
          String(cell).trim() !== ''
        )
      ) {
        rows.push(row);
      }

      if (rows.length < 2) return [];

      const headers = rows[0].map(
        header =>
          String(header)
            .trim()
            .toLowerCase()
      );

      return rows
        .slice(1)
        .map(cells => {
          const result = {};

          headers.forEach(
            (header, index) => {
              result[header] =
                String(cells[index] ?? '')
                  .trim();
            }
          );

          return result;
        });
    }

    function csvNumber(row, field) {
      const value = Number(
        String(row?.[field] ?? '')
          .replace(',', '.')
      );

      return Number.isFinite(value)
        ? value
        : NaN;
    }

    function csvBoolean(row, field) {
      const value = String(
        row?.[field] ?? ''
      ).trim().toLowerCase();

      return (
        value === '1' ||
        value === 'true' ||
        value === 'on' ||
        value === 'yes'
      );
    }

    function formatXdbReason(delta) {
      const parts = [
        [
          delta.start,
          tr('ош. запуска')
        ],
        [
          delta.status,
          tr('ош. статуса')
        ],
        [
          delta.timeout,
          tr('тайм-аутов')
        ],
        [
          delta.packet,
          tr('ош. пакета')
        ],
        [
          delta.rejected,
          tr('отбраковано')
        ]
      ]
        .filter(
          ([value]) =>
            Number.isFinite(value) &&
            value > 0
        )
        .map(
          ([value, label]) =>
            fmt(value, 0) +
            ' ' +
            label
        );

      return parts.length
        ? parts.join(' + ')
        : tr('свежие данные давления не поступали');
    }

    function counterDelta(
      before,
      after,
      field
    ) {
      const start = csvNumber(
        before,
        field
      );
      const end = csvNumber(
        after,
        field
      );

      if (
        !Number.isFinite(start) ||
        !Number.isFinite(end)
      ) {
        return NaN;
      }

      return Math.max(
        0,
        end - start
      );
    }

    function maxCsvValue(rows, field) {
      let maximum = NaN;

      rows.forEach(row => {
        const value = csvNumber(
          row,
          field
        );

        if (
          Number.isFinite(value) &&
          (
            !Number.isFinite(maximum) ||
            value > maximum
          )
        ) {
          maximum = value;
        }
      });

      return maximum;
    }

    function formatSensorAge(ageMs) {
      if (!Number.isFinite(ageMs)) {
        return '—';
      }

      return ageMs >= 1000
        ? (
            (ageMs / 1000)
              .toFixed(2)
              .replace('.', ',') +
            ' ' +
            tr('с')
          )
        : (
            fmt(ageMs, 0) +
            ' ' +
            tr('мс')
          );
    }

    function setLatestShotFacts({
      protection,
      eventLabel,
      eventValue,
      sensorAge,
      consecutive,
      recovery,
      reason
    }) {
      const values = {
        xdbShotProtection:protection,
        xdbShotEventLabel:eventLabel,
        xdbShotEventValue:eventValue,
        xdbShotSensorAge:sensorAge,
        xdbShotConsecutive:consecutive,
        xdbShotRecovery:recovery,
        xdbShotReason:reason
      };

      Object.entries(values)
        .forEach(([id, value]) => {
          const node = $(id);
          if (node) {
            node.textContent = value || '—';
          }
        });
    }

    function renderLatestShotAnalysis(rows) {
      const card = $('xdbLastShotCard');
      const state = $('xdbShotState');
      const summary = $('xdbShotSummary');
      const facts = $('xdbShotFacts');

      if (
        !card ||
        !state ||
        !summary ||
        !facts
      ) {
        return;
      }

      card.classList.remove(
        'loading',
        'problem',
        'clear'
      );

      if (!rows.length) {
        state.textContent =
          tr('CSV пуст или не распознан');
        summary.textContent =
          tr('В последнем файле нет строк, которые можно использовать для диагностики.');
        facts.hidden = true;
        return;
      }

      const failIndex = rows.findIndex(
        row => {
          const stateValue = String(
            row.startup_state || ''
          ).toUpperCase();

          return (
            !csvBoolean(
              row,
              'sensor_ok'
            ) ||
            stateValue.includes(
              'SENSOR_FAIL'
            )
          );
        }
      );

      const first = rows[0];
      const last = rows[
        rows.length - 1
      ];
      const totalDelta =
        counterDelta(
          first,
          last,
          'xdb_total_errors'
        );
      const shotMaxAge =
        maxCsvValue(
          rows,
          'sensor_age_ms'
        );
      const shotMaxConsecutive =
        maxCsvValue(
          rows,
          'xdb_consecutive_errors'
        );
      const shotDelta = {
        start:counterDelta(
          first,
          last,
          'xdb_start_errors'
        ),
        status:counterDelta(
          first,
          last,
          'xdb_status_errors'
        ),
        timeout:counterDelta(
          first,
          last,
          'xdb_measurement_timeouts'
        ),
        packet:counterDelta(
          first,
          last,
          'xdb_packet_errors'
        ),
        rejected:counterDelta(
          first,
          last,
          'xdb_rejected_packets'
        )
      };

      if (failIndex < 0) {
        card.classList.add('clear');
        state.textContent =
          tr('Защита SENSOR_FAIL не сработала');

        if (
          Number.isFinite(totalDelta) &&
          totalDelta > 0
        ) {
          summary.textContent =
            tr('Во время последнего пролива XDB401 зарегистрировал ошибки, но данные восстановились до срабатывания защиты и помпа не была принудительно остановлена.');
        } else {
          summary.textContent =
            tr('Последний записанный пролив прошёл без потери связи с XDB401 и без остановки помпы защитой.');
        }

        setLatestShotFacts({
          protection:tr('Не сработала'),
          eventLabel:tr('Ошибок за пролив'),
          eventValue:
            Number.isFinite(totalDelta)
              ? fmt(totalDelta, 0)
              : '—',
          sensorAge:
            formatSensorAge(
              shotMaxAge
            ),
          consecutive:
            Number.isFinite(
              shotMaxConsecutive
            )
              ? fmt(
                  shotMaxConsecutive,
                  0
                )
              : '—',
          recovery:
            Number.isFinite(totalDelta) &&
            totalDelta > 0
              ? tr('До порога защиты')
              : tr('Не требовалось'),
          reason:
            Number.isFinite(totalDelta) &&
            totalDelta > 0
              ? formatXdbReason(
                  shotDelta
                )
              : tr('Ошибок не было')
        });
        facts.hidden = false;
        return;
      }

      let incidentStartIndex =
        failIndex;

      while (
        incidentStartIndex > 0 &&
        csvNumber(
          rows[incidentStartIndex - 1],
          'xdb_consecutive_errors'
        ) > 0
      ) {
        incidentStartIndex -= 1;
      }

      if (
        incidentStartIndex > 0 &&
        csvNumber(
          rows[incidentStartIndex],
          'xdb_consecutive_errors'
        ) > 0
      ) {
        incidentStartIndex -= 1;
      }

      let lastFailIndex =
        failIndex;

      while (
        lastFailIndex + 1 <
          rows.length
      ) {
        const next =
          rows[lastFailIndex + 1];
        const stateValue =
          String(
            next.startup_state || ''
          ).toUpperCase();

        if (
          csvBoolean(
            next,
            'sensor_ok'
          ) &&
          !stateValue.includes(
            'SENSOR_FAIL'
          )
        ) {
          break;
        }

        lastFailIndex += 1;
      }

      const recoveryIndex =
        lastFailIndex + 1 <
        rows.length
          ? lastFailIndex + 1
          : -1;

      const before =
        rows[
          Math.max(
            0,
            incidentStartIndex
          )
        ];
      const failure =
        rows[failIndex];
      const failureEnd =
        recoveryIndex >= 0
          ? rows[recoveryIndex]
          : rows[lastFailIndex];

      const failureMs =
        csvNumber(
          failure,
          'elapsed_ms'
        );
      const recoveryMs =
        recoveryIndex >= 0
          ? csvNumber(
              rows[recoveryIndex],
              'elapsed_ms'
            )
          : NaN;

      const incidentRows =
        rows.slice(
          Math.max(
            0,
            incidentStartIndex
          ),
          (
            recoveryIndex >= 0
              ? recoveryIndex
              : lastFailIndex
          ) + 1
        );

      const maxAge = maxCsvValue(
        incidentRows,
        'sensor_age_ms'
      );

      const maxConsecutive =
        maxCsvValue(
          incidentRows,
          'xdb_consecutive_errors'
        );

      const pumpStopped =
        incidentRows.some(
          row =>
            csvNumber(
              row,
              'final_output_percent'
            ) <= .01
        );

      const incidentDelta = {
        start:counterDelta(
          before,
          failureEnd,
          'xdb_start_errors'
        ),
        status:counterDelta(
          before,
          failureEnd,
          'xdb_status_errors'
        ),
        timeout:counterDelta(
          before,
          failureEnd,
          'xdb_measurement_timeouts'
        ),
        packet:counterDelta(
          before,
          failureEnd,
          'xdb_packet_errors'
        ),
        rejected:counterDelta(
          before,
          failureEnd,
          'xdb_rejected_packets'
        )
      };

      const reason =
        formatXdbReason(
          incidentDelta
        );
      const eventTime =
        Number.isFinite(failureMs)
          ? (
              (failureMs / 1000)
                .toFixed(1)
                .replace('.', ',') +
              ' ' +
              tr('с')
            )
          : '—';
      const ageText =
        formatSensorAge(maxAge);
      const recoveryText =
        Number.isFinite(
          recoveryMs
        ) &&
        Number.isFinite(
          failureMs
        )
          ? (
              (
                (
                  recoveryMs -
                  failureMs
                ) / 1000
              )
                .toFixed(1)
                .replace('.', ',') +
              ' ' +
              tr('с')
            )
          : tr('не зафиксировано');

      card.classList.add('problem');
      state.textContent =
        tr('Обнаружено защитное отключение');

      summary.textContent =
        pumpStopped
          ? (
              tr('На') +
              ' ' +
              eventTime +
              ' ' +
              tr('контроллер потерял надёжные данные XDB401 и остановил помпу защитой SENSOR_FAIL. Максимальный возраст данных составил') +
              ' ' +
              ageText +
              '. ' +
              tr('Ошибок подряд') +
              ': ' +
              (
                Number.isFinite(
                  maxConsecutive
                )
                  ? fmt(
                      maxConsecutive,
                      0
                    )
                  : '—'
              ) +
              '.'
            )
          : (
              tr('В последнем CSV обнаружена потеря XDB401, но нулевой выход помпы в строках сбоя не найден.')
            );

      setLatestShotFacts({
        protection:'SENSOR_FAIL',
        eventLabel:tr('Момент отключения'),
        eventValue:eventTime,
        sensorAge:ageText,
        consecutive:
          Number.isFinite(
            maxConsecutive
          )
            ? fmt(maxConsecutive, 0)
            : '—',
        recovery:recoveryText,
        reason
      });
      facts.hidden = false;
    }

    async function analyzeLatestShotCsv(
      force = false
    ) {
      if (xdbLastShotLoading) return;

      if (
        !force &&
        Date.now() -
          xdbLastShotLoadedAt <
          15000
      ) {
        return;
      }

      const card = $('xdbLastShotCard');
      const state = $('xdbShotState');

      if (!card || !state) return;

      xdbLastShotLoading = true;
      card.classList.remove(
        'problem',
        'clear'
      );
      card.classList.add('loading');
      state.textContent =
        tr('Проверяем запись последнего пролива…');

      try {
        const response = await fetch(
          '/shot.csv?diagnostic=' +
            Date.now(),
          {
            cache:'no-store'
          }
        );

        if (!response.ok) {
          throw new Error(
            'CSV unavailable'
          );
        }

        const csvText =
          await response.text();

        const rows =
          parseCsvRows(csvText);

        renderLatestShotAnalysis(rows);
        xdbLastShotLoadedAt =
          Date.now();
      } catch (_) {
        card.classList.remove(
          'loading',
          'problem',
          'clear'
        );
        state.textContent =
          tr('Последний CSV недоступен');
        $('xdbShotSummary').textContent =
          tr('Запишите диагностический пролив, после чего здесь появится автоматический разбор связи XDB401 и остановки помпы.');
        $('xdbShotFacts').hidden = true;
      } finally {
        xdbLastShotLoading = false;
      }
    }

    function health() {
      const consecutive = Number($('errConsecutive')?.textContent);
      const dataAge = Number($('pressureDataAge')?.textContent);
      const ok =
        Number.isFinite(consecutive) &&
        consecutive === 0 &&
        Number.isFinite(dataAge) &&
        dataAge <= 1;
      ['sensorHealth'].forEach(id => {
        const node = $(id);
        if (!node) return;
        node.textContent = ok ? 'OK' : tr('Ошибка');
        node.style.color = ok ? 'var(--green)' : 'var(--red)';
      });
      updateXdbHumanStatus();
      updateHomeDashboard();
    }

    function stateOn(value) {
      return value === true || String(value).toUpperCase() === 'ON' || String(value).toLowerCase() === 'true';
    }

    function renderStatusValue(elementId, type, value) {
      const node = $(elementId);
      if (type === 'toggle') {
        if (node) node.checked = stateOn(value);
        if (elementId === 'stopByWeight') updateStopByWeightControl();
        return;
      }
      if (type === 'status_bool') {
        const on = stateOn(value);
        if (node) {
          if (elementId === 'warmupStatus') {
            node.textContent = tr(on ? 'Отключён на прогреве' : 'Работает');
            node.style.color = on ? 'var(--yellow)' : 'var(--green)';
          } else {
            node.textContent = tr(on ? 'Да' : 'Нет');
            node.style.color = on ? 'var(--green)' : 'var(--muted)';
          }
        }
        return;
      }
      if (!node) return;
      if (type === 'num0') node.textContent = fmt(value, 0);
      else if (type === 'num') {
        const numeric = Number(value);
        node.textContent =
          elementId === 'pressure' && Number.isFinite(numeric)
            ? fmt(Math.max(0, numeric))
            : fmt(value);
      }
      else if (type === 'seconds') node.textContent = fmt(value, 0) + ' ' + tr('с');
      else {
        if (elementId === 'shotStatus') {
          node.dataset.rawValue =
            String(value ?? 'Ожидание');

          if (shotRunning) {
            updateShotPhaseStatus();
            return;
          }
        }

        if (
          elementId === 'pt100Status' ||
          elementId === 'scaleDiagStatus'
        ) {
          const labels = {
            OK:'OK',
            NO_DATA:tr('Нет данных'),
            STALE:tr('Данные перестали обновляться'),
            INVALID:tr('Некорректное значение'),
            SUDDEN_DROP:tr('Резкое падение к нулю'),
            NOT_TARED:tr('Не выполнена тара'),
            NOT_CALIBRATED:tr('Не откалиброваны')
          };
          const raw = String(value ?? '').trim().toUpperCase();
          node.textContent = labels[raw] || localizeDynamicText(value);
          node.style.color = raw === 'OK'
            ? 'var(--green)'
            : raw === 'NOT_TARED' || raw === 'NOT_CALIBRATED'
              ? 'var(--accent)'
              : 'var(--red)';
        } else if (elementId === 'autotuneStatus') {
          const rawAutotune = String(value ?? '').trim();
          if (/^idle$/i.test(rawAutotune)) node.textContent = tr('Не запущена');
          else if (/^(running|tuning|active)$/i.test(rawAutotune)) node.textContent = tr('Выполняется');
          else if (/^completed/i.test(rawAutotune)) node.textContent = tr('Завершена');
          else node.textContent = localizeDynamicText(value);
        } else {
          node.textContent = localizeDynamicText(value);
        }
      }

      if (
        xdbDiagnosticIds.includes(
          elementId
        )
      ) {
        updateXdbHumanStatus();
      }
    }

    function updateDiagnosticMirrors(elementId, value) {
      const targets = {
        pressure: ['diagPressure'],
        target: ['diagTarget'],
        pump: ['diagPump'],
        temp: ['diagTemp'],
        heat: ['diagHeat'],
        weight: ['scaleDiagnosticWeight'],
        warmupStatus: ['diagWarmup'],
        autotuneStatus: ['diagAutotune']
      }[elementId] || [];
      targets.forEach(id => {
        const node = $(id);
        if (!node) return;
        if (elementId === 'warmupStatus') {
          const on = stateOn(value);
          node.textContent = tr(on ? 'Активен' : 'Нет');
          node.style.color = on ? 'var(--accent)' : 'var(--muted)';
        } else if (elementId === 'autotuneStatus') {
          node.textContent = localizeDynamicText(value);
        } else {
          node.textContent = fmt(value);
        }
      });
    }

    function apply(event) {
      const key = clean(event.name_id || event.id);
      const exactPidStateHandled = applyExactPidStateFromSse(key, event);
      const numberStateHandled = applyNumberStateFromSse(key, event);
      const hit = Object.entries(E).find(([name]) => key.endsWith(name));

      if (!hit) {
        if (numberStateHandled || exactPidStateHandled) {
          updateHomeDashboard();
          updateQuickActions();
          health();
        }
        return;
      }

      const [elementId, type] = hit[1];
      const value = event.value ?? event.state;
      const node = $(elementId);

      if (type === 'bool') {
        const on = stateOn(value);
        if (elementId === 'shotSwitch') setShotRunning(on);
        if (elementId === 'powerState') showPower(on);
        if (elementId === 'hotWaterState') setHotWater(on);
        if (elementId === 'steamModeState') setSteamMode(on);
        if (elementId === 'waterLevelState') showWaterLevel(on);
        updateHomeDashboard();
        return;
      }

      if (type === 'num' || type === 'num0') {
        const number = Number(value);
        values[elementId] = number;
        if (elementId === 'weight') {
          observeShotWeight(number);
        }
        if (elementId === 'target') updatePressureTarget();
        else renderStatusValue(elementId, type, number);
        updateDiagnosticMirrors(elementId, number);
      } else if (type === 'toggle' || type === 'status_bool') {
        renderStatusValue(elementId, type, value);
        updateDiagnosticMirrors(elementId, value);
      } else if (type === 'seconds') {
        renderStatusValue(elementId, type, value);
      } else {
        if (elementId === 'profile') {
          const nextProfile = String(value ?? '').trim();
          const fetchedOptions = profileOptionsFromData(event);

          if (fetchedOptions.length) {
            availableProfileOptions = fetchedOptions;
          } else if (nextProfile && !availableProfileOptions.includes(nextProfile)) {
            // Normal DETAIL_STATE updates do not repeat Select options. Keep a
            // newly observed option visible without requiring a REST detail GET.
            availableProfileOptions = [...availableProfileOptions, nextProfile];
          }

          const profileChanged =
            !!nextProfile &&
            nextProfile !== currentProfileName;

          if (nextProfile) currentProfileName = nextProfile;
          syncProfileControls(currentProfileName);
          updateShotScenario();
          updateChartControls();

          // The preset's Number events may arrive just before or just after the
          // Select event. Debouncing both sides gives the graph one coherent,
          // near-instant redraw while SSE remains the source of truth.
          if (profileChanged || fetchedOptions.length) {
            scheduleProfileStateCommitFromSse();
          }
        } else {
          renderStatusValue(elementId, type, value);
        }
        updateDiagnosticMirrors(elementId, value);
      }

      if (elementId === 'shotStatus' && node) node.classList.toggle('run', String(value).toLowerCase().includes('пролив'));
      if (['pressure', 'target', 'flow', 'pump', 'temp', 'tempTarget', 'heat'].includes(elementId)) sample();
      updateHomeDashboard(); updateQuickActions(); health();
    }

    function activeTemperatureTarget() {
      const brew = Number(values.tempTarget);
      const steam = Number(values.steamTargetLive);
      return steamModeOn && Number.isFinite(steam) ? steam : brew;
    }

    function sample() {
      const now = performance.now();
      if (now - lastPoint < 170) return;
      lastPoint = now;

      if (shotRunning) {
        liveSeries.push({
          x: Math.max(0, (now - liveStart) / 1000),
          p: Number(values.pressure) || 0,
          t: Number(values.target) || 0,
          f: Number(values.flow) || 0,
          u: Number(values.pump) || 0
        });
        if (liveSeries.length > 900) liveSeries.shift();
        draw();
      }

      if (now - lastTempPoint >= 1000) {
        lastTempPoint = now;
        const nowSeconds = now / 1000;
        tempSeries.push({
          x: nowSeconds,
          v: Number(values.temp) || 0,
          t: Number(activeTemperatureTarget()) || 0,
          h: Number(values.heat) || 0
        });
        const cutoff = nowSeconds - maxTempHistorySeconds - 15;
        while (tempSeries.length && Number(tempSeries[0].x) < cutoff) tempSeries.shift();
        if (tempSeries.length > 900) tempSeries.splice(0, tempSeries.length - 900);
        drawTemp();
       
      }
    }

    function builtInProfileRampSeconds(profileName, mainSeconds) {
      const profile = String(profileName || '');
      let seconds = NaN;

      if (profile === 'Slayer Style') seconds = 5;
      else if (profile === 'Bloom') seconds = 4;
      else if (profile === 'Classic' || profile === 'Lever') seconds = 3;

      if (!Number.isFinite(seconds)) return NaN;
      return Math.min(Math.max(0, Number(mainSeconds) || 0), seconds);
    }

    function displayedProfileRampSeconds(config, profileName) {
      const mainSeconds = Math.max(0, Number(config?.shotSeconds) || 0);
      const configured = Math.min(
        mainSeconds,
        Math.max(0, Number(config?.softInfusionTime) || 0)
      );
      const builtIn = builtInProfileRampSeconds(profileName, mainSeconds);
      const editing = !!(
        recipeDirty ||
        mainCurveDirty ||
        mainCurveEditing ||
        mainCurveDragKey
      );

      return !editing && Number.isFinite(builtIn)
        ? builtIn
        : configured;
    }

    function buildProfileSeries(
      config = profileCfg,
      profileName = currentProfileName || 'Custom'
    ) {
      const phaseModel = displayedAdvancedPhases(profileName, config);
      if (phaseModel) return advancedPhaseSeries(phaseModel);
      const editing = !!(
        recipeDirty ||
        mainCurveDirty ||
        mainCurveEditing ||
        mainCurveDragKey
      );
      if (String(profileName || '') === 'Funnel' && !editing) {
        const total = 200;
        const steps = 360;
        const result = [];
        for (let index = 0; index <= steps; index++) {
          const x = total * index / steps;
          let target = 0;
          if (x < 12) target = 0.5;
          else if (x >= 40 && x < 70) target = 0.8;
          else if (x >= 85 && x < 115) target = 0.8;
          else if (x >= 130 && x < 160) target = 0.8;
          result.push({ x, t: target });
        }
        return result;
      }

      const prePump = Math.max(0, Number(config.prePump) || 0);
      const prePause = Math.max(0, Number(config.prePause) || 0);
      const mainSeconds = Math.max(0, Number(config.shotSeconds) || 0);
      const total = prePump + prePause + mainSeconds;
      if (total <= 0) return [];

      const mainPressure = Math.max(0, Number(config.mainPressure) || 0);
      const classicProfile = String(profileName || '') === 'Classic';
      const startPressure = classicProfile
        ? mainPressure
        : Math.max(0, Number(config.prePower) || 0);
      const endPressure = classicProfile
        ? mainPressure
        : Math.max(0, Number(config.endPressure) || 0);
      const rampSeconds = displayedProfileRampSeconds(config, profileName);
      const finishSeconds = Math.max(0, mainSeconds - rampSeconds);
      const smoothstep = value => {
        const x = Math.max(0, Math.min(1, value));
        return x * x * (3 - 2 * x);
      };
      const steps = Math.max(10, Math.min(360, Math.ceil(total * 5)));
      const result = [];

      for (let index = 0; index <= steps; index++) {
        const x = total * index / steps;
        let target = 0;
        if (x < prePump) {
          target = startPressure;
        } else if (x < prePump + prePause) {
          target = 0;
        } else if (mainSeconds > 0) {
          const brewElapsed = Math.max(0, x - prePump - prePause);
          if (brewElapsed <= rampSeconds && rampSeconds > 0) {
            target = startPressure + (mainPressure - startPressure) * smoothstep(brewElapsed / rampSeconds);
          } else if (finishSeconds > 0) {
            const finishElapsed = brewElapsed - rampSeconds;
            target = mainPressure + (endPressure - mainPressure) * smoothstep(finishElapsed / finishSeconds);
          } else {
            target = mainPressure;
          }
        }
        result.push({ x, t: target });
      }
      return result;
    }

    function profileTargetAt(series, seconds) {
      if (!series.length) return NaN;
      const x = Number(seconds);
      if (!Number.isFinite(x)) return Number(series[0]?.t);
      if (x <= Number(series[0]?.x || 0)) return Number(series[0]?.t);
      for (let index = 1; index < series.length; index++) {
        const previous = series[index - 1];
        const next = series[index];
        const x0 = Number(previous.x);
        const x1 = Number(next.x);
        if (!Number.isFinite(x0) || !Number.isFinite(x1)) continue;
        if (x <= x1) {
          const t0 = Number(previous.t);
          const t1 = Number(next.t);
          if (!Number.isFinite(t0)) return t1;
          if (!Number.isFinite(t1) || x1 <= x0) return t0;
          const ratio = Math.max(0, Math.min(1, (x - x0) / (x1 - x0)));
          return t0 + (t1 - t0) * ratio;
        }
      }
      return Number(series[series.length - 1]?.t);
    }

    function seriesContainsFact(series) {
      return series.some(point =>
        Number.isFinite(Number(point.p)) ||
        Number.isFinite(Number(point.f)) ||
        Number.isFinite(Number(point.u))
      );
    }

    function chartSeries() {
      if (chartMode === 'archive') return archiveSeries;

      const displayConfig = profileDisplayConfig();
      const displayProfile = (
        recipeDirty ||
        mainCurveDirty ||
        mainCurveEditing ||
        mainCurveDragKey
      ) ? 'Custom' : (currentProfileName || 'Custom');
      const profileSeries = buildProfileSeries(displayConfig, displayProfile);
      /*
       * Keep the profile editor independent from the previous extraction.
       * A completed shot has its own view in the "Shots" tab; reusing its
       * samples here stretches the time axis and leaves profile handles away
       * from their curve after a page restore or a profile change.
       * Live samples are still overlaid while an extraction is running.
       */
      const factSeries = shotRunning
        ? liveSeries
        : (shotCompletedThisSession ? lastShotSeries : []);

      if (!factSeries.length) return profileSeries;

      const combined = factSeries.map((point, index) => {
        const seconds = Number.isFinite(Number(point.x)) ? Number(point.x) : index * .2;
        return {
          ...point,
          x:seconds,
          t:profileTargetAt(profileSeries, seconds)
        };
      });

      const lastFactX = combined.length
        ? Number(combined[combined.length - 1].x)
        : -1;

      profileSeries.forEach(point => {
        const x = Number(point.x);
        if (Number.isFinite(x) && x > lastFactX + .001) {
          combined.push({
            x,
            t:Number(point.t),
            p:NaN,
            f:NaN,
            u:NaN
          });
        }
      });

      return combined;
    }

    function updateChartControls() {
      const dirty = !!(mainCurveDirty || recipeDirty);
      const shownProfile = dirty ? 'Custom' : currentProfileName;
      const select = $('homeProfileSelect');

      if (
        select &&
        shownProfile &&
        select.value !== shownProfile &&
        [...select.options].some(option => option.value === shownProfile)
      ) {
        select.value = shownProfile;
      }

      const context = $('chartContext');
      if (context) {
        context.textContent =
          chartMode === 'archive'
            ? (
                archiveTitle ||
                tr('Сохранённый пролив')
              )
            : (
                tr('Профиль') +
                ' · ' +
                (shownProfile
                  ? profileLabel(shownProfile) + (dirty ? ' *' : '')
                  : '—')
              );
      }

      updateHomeProfileRail(availableProfileOptions, shownProfile);
    }

    function canvasFrame(id, margins) {
      const canvas = $(id);
      const rect = canvas.getBoundingClientRect();
      const dpr = devicePixelRatio || 1;
      canvas.width = Math.max(1, Math.round(rect.width * dpr));
      canvas.height = Math.max(1, Math.round(rect.height * dpr));
      const ctx = canvas.getContext('2d');
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      ctx.clearRect(0, 0, rect.width, rect.height);
      return {
        ctx,
        w: rect.width,
        h: rect.height,
        left: margins.left,
        right: rect.width - margins.right,
        top: margins.top,
        bottom: rect.height - margins.bottom,
        plotW: Math.max(1, rect.width - margins.left - margins.right),
        plotH: Math.max(1, rect.height - margins.top - margins.bottom)
      };
    }

    function yAt(frame, value, min, max) {
      const clamped = Math.max(min, Math.min(max, Number(value)));
      return frame.bottom - (clamped - min) / (max - min) * frame.plotH;
    }

    function xRange(series) {
      if (!series.length) return 1;
      const last = Math.max(...series.map((point, index) => Number.isFinite(Number(point.x)) ? Number(point.x) : index));
      return Math.max(1, last);
    }

    function xAt(frame, point, index, series, maxX) {
      const value = Number.isFinite(Number(point.x)) ? Number(point.x) : index / Math.max(1, series.length - 1) * maxX;
      return frame.left + value / maxX * frame.plotW;
    }

    function plotLine(frame, series, key, color, min, max, width = 2, dashed = false, opacity = 1) {
      if (series.length < 2) return;
      const maxX = xRange(series);
      const ctx = frame.ctx;
      ctx.save();
      ctx.beginPath();
      ctx.rect(frame.left, frame.top, frame.plotW, frame.plotH);
      ctx.clip();
      ctx.strokeStyle = color;
      ctx.globalAlpha = Math.max(
        0,
        Math.min(
          1,
          Number(opacity) || 0
        )
      );
      ctx.lineWidth = width;
      ctx.lineJoin = 'round';
      ctx.lineCap = 'round';
      ctx.setLineDash(dashed ? [12, 8] : []);
      ctx.beginPath();
      let started = false;
      series.forEach((point, index) => {
        const value = Number(point[key]);
        if (!Number.isFinite(value)) { started = false; return; }
        const x = xAt(frame, point, index, series, maxX);
        const y = yAt(frame, value, min, max);
        if (!started) { ctx.moveTo(x, y); started = true; }
        else ctx.lineTo(x, y);
      });
      ctx.stroke();
      ctx.restore();
    }

    function drawTimeAxis(frame, series) {
      const ctx = frame.ctx;
      const maxX = xRange(series);
      /*
       * Five-second labels are useful for normal espresso profiles, but on a
       * long recipe (Funnel is 200 s including pauses and drawdown) they turn
       * into an unreadable row of overlapping numbers. Keep the familiar
       * 5-second scale while it fits and select the next human-friendly step
       * only when the available canvas width requires it.
       */
      const minimumTickSpacing = 48;
      const maximumTickCount = Math.max(
        2,
        Math.floor(frame.plotW / minimumTickSpacing)
      );
      const minimumStep = maxX / maximumTickCount;
      const friendlySteps = [5, 10, 15, 20, 30, 45, 60, 90, 120];
      const step = friendlySteps.find(candidate => candidate >= minimumStep) ||
        Math.ceil(minimumStep / 60) * 60;
      const ticks = [];

      for (let second = 0; second < maxX - 0.001; second += step) {
        ticks.push(second);
      }

      /*
       * The exact recipe duration belongs to the time axis, even when it is
       * not a multiple of the friendly step. Its label has priority, but the
       * regular grid lines remain visible even if a nearby label must hide.
       */
      if (maxX > 0) ticks.push(maxX);

      ctx.save();
      ctx.font = chartFont(11);
      const tickText = second => fmt(second, 0) + ' ' + tr('с');
      const hiddenTickLabels = new Set();

      if (ticks.length > 1 && maxX > 0) {
        const labelGap = 10;
        const finalLabelLeft = frame.right -
          ctx.measureText(tickText(maxX)).width;

        for (let index = ticks.length - 2; index > 0; index -= 1) {
          const second = ticks[index];
          const x = frame.left + second / maxX * frame.plotW;
          const labelRight = x +
            ctx.measureText(tickText(second)).width / 2;
          if (labelRight + labelGap <= finalLabelLeft) break;
          hiddenTickLabels.add(index);
        }
      }

      ticks.forEach((second, index) => {
        const ratio = maxX > 0 ? second / maxX : 0;
        const x = frame.left + ratio * frame.plotW;

        ctx.strokeStyle = 'rgba(42,48,58,.72)';
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(x, frame.top);
        ctx.lineTo(x, frame.bottom);
        ctx.stroke();

        const isFinal = maxX > 0 && Math.abs(second - maxX) < 0.01;
        ctx.fillStyle = isFinal ? '#e6a064' : '#7f8793';
        ctx.textBaseline = 'top';
        ctx.textAlign = index === 0 ? 'left' : isFinal ? 'right' : 'center';
        if (!hiddenTickLabels.has(index)) {
          ctx.fillText(tickText(second), x, frame.bottom + 5);
        }
      });
      ctx.restore();
    }

    function drawPressureAxes(frame, series) {
      const ctx = frame.ctx;
      ctx.font = chartFont(11);
      ctx.textBaseline = 'middle';

      for (let bar = 0; bar <= 12; bar += 2) {
        const y = yAt(frame, bar, 0, 12);
        ctx.strokeStyle = '#2a303a';
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(frame.left, y);
        ctx.lineTo(frame.right, y);
        ctx.stroke();

        ctx.fillStyle = '#d5dae1';
        ctx.textAlign = 'right';
        ctx.fillText(
          bar + ' bar',
          frame.left - 7,
          y
        );
      }

      for (let flow = 0; flow <= 6; flow += 1) {
        const y = yAt(frame, flow, 0, 6);
        ctx.fillStyle = '#4bd18b';
        ctx.textAlign = 'left';
        ctx.fillText(
          String(flow),
          frame.right + 9,
          y
        );
      }

      for (let power = 0; power <= 100; power += 20) {
        const y = yAt(frame, power, 0, 100);
        ctx.fillStyle = '#a98af2';
        ctx.textAlign = 'right';
        ctx.fillText(
          power + '%',
          frame.w - 3,
          y
        );
      }

      drawTimeAxis(frame, series);
    }

    function hideChartTooltip() {
      const tooltip = $('chartTooltip');
      if (tooltip) tooltip.hidden = true;
    }

    function nearestChartPoint(series, targetX) {
      if (!series.length) return -1;
      let nearest = 0;
      let distance = Infinity;
      series.forEach((point, index) => {
        const x = Number.isFinite(Number(point.x)) ? Number(point.x) : index;
        const current = Math.abs(x - targetX);
        if (current < distance) {
          distance = current;
          nearest = index;
        }
      });
      return nearest;
    }

    function chartTooltipRow(label, value, unit, color, dashed = false) {
      if (!Number.isFinite(Number(value))) return '';
      return '<div class="chart-tooltip-row">' +
        '<i class="chart-tooltip-mark' + (dashed ? ' dashed' : '') + '" style="color:' + color + '"></i>' +
        '<span>' + tr(label) + '</span>' +
        '<b>' + value + (unit ? ' ' + unit : '') + '</b>' +
      '</div>';
    }

    function updateChartTooltip(frame, series, index) {
      const tooltip = $('chartTooltip');
      const canvas = $('chart');
      if (!tooltip || !canvas || index < 0 || index >= series.length) {
        hideChartTooltip();
        return;
      }

      const point = series[index];
      const maxX = xRange(series);
      const pointX = xAt(frame, point, index, series, maxX);
      const seconds = Number.isFinite(Number(point.x))
        ? Number(point.x)
        : index / Math.max(1, series.length - 1) * maxX;

      let rows = chartTooltipRow('Профиль', fmt(point.t, 2), tr('бар'), '#ffd166', true);
      if (Number.isFinite(Number(point.p)) || Number.isFinite(Number(point.f)) || Number.isFinite(Number(point.u))) {
        rows += chartTooltipRow('Факт', fmt(point.p, 2), tr('бар'), '#54a9ff');
        rows += chartTooltipRow('Поток', fmt(point.f, 2), tr('г/с'), '#4bd18b');
        rows += chartTooltipRow('Помпа', fmt(point.u, 1), '%', 'rgba(154,114,239,.75)');
      }

      tooltip.innerHTML =
        '<div class="chart-tooltip-head"><span>' + tr('Секунды') + '</span><b>' +
        fmt(seconds, seconds < 10 ? 1 : 0) + ' ' + tr('с') +
        '</b></div>' + rows;
      tooltip.hidden = false;

      const preferredY = Number.isFinite(Number(point.p))
        ? yAt(frame, point.p, 0, 12)
        : yAt(frame, point.t, 0, 12);
      const canvasTop = canvas.offsetTop;
      const canvasLeft = canvas.offsetLeft;
      const gap = 12;
      const tooltipWidth = tooltip.offsetWidth;
      const tooltipHeight = tooltip.offsetHeight;
      const parentWidth = canvas.parentElement.clientWidth;

      let left = canvasLeft + pointX + gap;
      if (left + tooltipWidth > parentWidth - 4) {
        left = canvasLeft + pointX - tooltipWidth - gap;
      }
      left = Math.max(4, Math.min(parentWidth - tooltipWidth - 4, left));

      let top = canvasTop + preferredY - tooltipHeight / 2;
      top = Math.max(canvasTop + 4, Math.min(canvasTop + canvas.clientHeight - tooltipHeight - 4, top));

      tooltip.style.left = left + 'px';
      tooltip.style.top = top + 'px';
    }

    function drawChartHover(frame, series) {
      if (chartHoverIndex < 0 || chartHoverIndex >= series.length) {
        hideChartTooltip();
        return;
      }

      const point = series[chartHoverIndex];
      const maxX = xRange(series);
      const x = xAt(frame, point, chartHoverIndex, series, maxX);
      const ctx = frame.ctx;

      ctx.save();
      ctx.beginPath();
      ctx.rect(frame.left, frame.top, frame.plotW, frame.plotH);
      ctx.clip();

      ctx.strokeStyle = 'rgba(255,255,255,.22)';
      ctx.lineWidth = 1;
      ctx.setLineDash([3, 4]);
      ctx.beginPath();
      ctx.moveTo(x, frame.top);
      ctx.lineTo(x, frame.bottom);
      ctx.stroke();
      ctx.setLineDash([]);

      const marks = [
        ['t', '#ffd166', 0, 12, 4.4],
        ['p', '#54a9ff', 0, 12, 4],
        ['f', '#4bd18b', 0, 6, 3.8],
        ['u', 'rgba(154,114,239,.75)', 0, 100, 3.6]
      ];

      marks.forEach(([key, color, min, max, radius]) => {
        const value = Number(point[key]);
        if (!Number.isFinite(value)) return;
        const y = yAt(frame, value, min, max);
        ctx.fillStyle = '#171a20';
        ctx.strokeStyle = color;
        ctx.lineWidth = 2;
        ctx.beginPath();
        ctx.arc(x, y, radius, 0, Math.PI * 2);
        ctx.fill();
        ctx.stroke();
      });

      ctx.restore();
      updateChartTooltip(frame, series, chartHoverIndex);
    }

    function handleChartPointer(event) {
      const canvas = $('chart');
      const series = chartSeries();
      if (!canvas || !series.length) {
        chartHoverIndex = -1;
        hideChartTooltip();
        return;
      }

      const rect = canvas.getBoundingClientRect();
      const left = 36;
      const right = rect.width - 60;
      const plotWidth = Math.max(1, right - left);
      const pointerX = Math.max(left, Math.min(right, event.clientX - rect.left));
      const targetX = (pointerX - left) / plotWidth * xRange(series);
      chartHoverIndex = nearestChartPoint(series, targetX);
      draw();
    }


    function drawRecipePressureAxes(frame, series) {
      const ctx = frame.ctx;
      ctx.font = chartFont(11);
      ctx.textBaseline = 'middle';

      for (let bar = 0; bar <= 12; bar += 2) {
        const y = yAt(frame, bar, 0, 12);
        ctx.strokeStyle = '#2a303a';
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(frame.left, y);
        ctx.lineTo(frame.right, y);
        ctx.stroke();

        ctx.fillStyle = '#d5dae1';
        ctx.textAlign = 'right';
        ctx.fillText(
          bar + ' bar',
          frame.left - 7,
          y
        );
      }

      drawTimeAxis(frame, series);
    }

    function drawRecipeBands(frame, config, series, profilePhases = null) {
      if (!series.length) return;

      const ctx = frame.ctx;
      const chartMax = Math.max(1, xRange(series));
      const preDuration = Math.max(
        0,
        Number(config.prePump) || 0
      );
      const pauseDuration = Math.max(
        0,
        Number(config.prePause) || 0
      );
      const mainDuration = Math.max(
        0,
        Number(config.shotSeconds) || 0
      );
      const profileTotal =
        preDuration +
        pauseDuration +
        mainDuration;
      const preEnd = preDuration;
      const pauseEnd = preEnd + pauseDuration;

      const xFor = seconds =>
        frame.left +
        Math.max(0, Math.min(chartMax, seconds)) /
          chartMax *
          frame.plotW;

      let stages;
      const detailedPhases = normalizeAdvancedPhases(profilePhases);
      if (detailedPhases) {
        let elapsed = 0;
        stages = detailedPhases.map((phase, index, all) => {
          const duration = Math.max(.5, phaseNumber(phase.duration));
          const from = elapsed;
          elapsed += duration;
          if (phase.type === 'pause') {
            return {
              label:'Пауза',
              shortLabel:'Пауза',
              duration,
              from,
              to:elapsed
            };
          }
          const explicitPreinfusion = phase.type === 'preinfusion';
          const phaseLabel = explicitPreinfusion
            ? 'Предсмачивание'
            : phaseDisplayName(phase, index, all);
          return {
            label:phaseLabel,
            shortLabel:explicitPreinfusion ? 'Смачив.' : 'Подача',
            duration,
            from,
            to:elapsed
          };
        });
      } else {
        stages = [
          {
            label:'Предсмачивание',
            shortLabel:'Предсмач.',
            duration:preDuration,
            from:0,
            to:preEnd
          },
          {
            label:'Пауза',
            shortLabel:'Пауза',
            duration:pauseDuration,
            from:preEnd,
            to:pauseEnd
          },
          {
            label:'Пролив',
            shortLabel:'Пролив',
            duration:mainDuration,
            from:pauseEnd,
            to:profileTotal
          }
        ].filter(stage => stage.to > stage.from + .001);
      }

      if (!stages.length) return;

      const stageTimeText = stage =>
        fmt(
          stage.duration,
          stage.duration % 1 ? 1 : 0
        ) +
        ' ' +
        tr('с');

      const chooseStageLabel = (stage, width) => {
        const timeText = stageTimeText(stage);
        const variants = [
          tr(stage.label) + ' · ' + timeText,
          tr(stage.shortLabel) + ' · ' + timeText,
          tr(stage.shortLabel),
          timeText
        ];
        const padding = 10;

        for (const variant of variants) {
          if (
            ctx.measureText(variant).width +
              padding <=
            width
          ) {
            return variant;
          }
        }

        return '';
      };

      /*
       * Stage 180: Decent-style recipe timeline.
       * Stage 181: the strip sits closer to the summary fields so the plot
       * gains vertical space; dotted stage guides are user-configurable.
       * Stages are shown as a neutral arrow strip above the plotting area.
       * The old colored background bands are intentionally gone: color on
       * the graph is reserved for data/profile lines and editable handles.
       */
      const lineY = Math.max(25, frame.top - 13);
      const labelY = Math.max(7, lineY - 17);

      ctx.save();
      ctx.font = chartFont(9);
      ctx.lineWidth = 1;
      ctx.strokeStyle = 'rgba(151,162,175,.70)';
      ctx.fillStyle = 'rgba(205,213,223,.78)';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'top';

      stages.forEach((stage, stageIndex) => {
        const left = xFor(stage.from);
        const right = xFor(stage.to);
        const width = right - left;
        if (width <= 1) return;

        const innerLeft = Math.min(right, left + Math.min(8, width * .22));
        const innerRight = Math.max(left, right - Math.min(8, width * .22));
        const arrow = Math.min(4, Math.max(2, width * .08));

        // Boundary ticks make adjacent stages read as |<---->|<---->|.
        ctx.beginPath();
        ctx.moveTo(left + .5, lineY - 7);
        ctx.lineTo(left + .5, lineY + 7);
        ctx.moveTo(right - .5, lineY - 7);
        ctx.lineTo(right - .5, lineY + 7);
        ctx.stroke();

        if (width >= 14) {
          ctx.beginPath();
          ctx.moveTo(innerLeft, lineY);
          ctx.lineTo(innerRight, lineY);
          ctx.stroke();

          // Left-facing arrowhead.
          ctx.beginPath();
          ctx.moveTo(innerLeft, lineY);
          ctx.lineTo(innerLeft + arrow, lineY - arrow);
          ctx.moveTo(innerLeft, lineY);
          ctx.lineTo(innerLeft + arrow, lineY + arrow);
          ctx.stroke();

          // Right-facing arrowhead.
          ctx.beginPath();
          ctx.moveTo(innerRight, lineY);
          ctx.lineTo(innerRight - arrow, lineY - arrow);
          ctx.moveTo(innerRight, lineY);
          ctx.lineTo(innerRight - arrow, lineY + arrow);
          ctx.stroke();
        }

        if (width >= 22) {
          const label = chooseStageLabel(stage, width);
          if (label) {
            ctx.save();
            ctx.beginPath();
            ctx.rect(
              left + 1,
              labelY - 1,
              Math.max(0, width - 2),
              14
            );
            ctx.clip();
            ctx.fillText(label, left + width / 2, labelY);
            ctx.restore();
          }
        }

        if (stageIndex > 0) {
          const stageGuide = shotLineStyle('stages');
          ctx.save();
          ctx.strokeStyle = stageGuide.color;
          ctx.lineWidth = stageGuide.width;
          ctx.globalAlpha = stageGuide.opacity;
          ctx.setLineDash([4, 5]);
          ctx.beginPath();
          ctx.moveTo(left, frame.top);
          ctx.lineTo(left, frame.bottom);
          ctx.stroke();
          ctx.restore();
        }
      });

      ctx.restore();
    }

    function archiveStageConfig(summary, series) {
      const total = Math.max(
        0,
        Number(xRange(series)) || 0,
        Number(archiveDuration(summary)) || 0
      );

      if (total <= 0) {
        return {
          prePump:0,
          prePause:0,
          shotSeconds:0
        };
      }

      const exactPre = archiveSummaryValue(
        summary,
        [
          'recipe.preinfusion_s',
          'recipe.pre_pump_s',
          'profile_config.prePump',
          'profile.prePump',
          'preinfusion_s',
          'pre_pump_s'
        ],
        NaN
      );
      const exactPause = archiveSummaryValue(
        summary,
        [
          'recipe.pause_s',
          'recipe.pre_pause_s',
          'profile_config.prePause',
          'profile.prePause',
          'pause_s',
          'pre_pause_s'
        ],
        NaN
      );

      let prePump = Number.isFinite(exactPre)
        ? Math.max(0, exactPre)
        : NaN;
      let prePause = Number.isFinite(exactPause)
        ? Math.max(0, exactPause)
        : NaN;

      /*
       * Older archive JSON files did not store phase durations. The CSV still
       * contains the pressure target, so for Custom/unknown profiles we can
       * reconstruct the visible stage boundaries without inventing recipe
       * values: a zero-target interval is Pause; otherwise the end of the
       * initial flat target marks the transition from Preinfusion to Brew.
       */
      if (
        (!Number.isFinite(prePump) || !Number.isFinite(prePause)) &&
        Array.isArray(series) &&
        series.length >= 3
      ) {
        const usable = series
          .map(point => ({
            x:Number(point.x),
            t:Number(point.t)
          }))
          .filter(point =>
            Number.isFinite(point.x) &&
            Number.isFinite(point.t)
          )
          .sort((a, b) => a.x - b.x);

        if (usable.length >= 3) {
          const positiveThreshold = .15;
          let pauseStart = NaN;
          let pauseEnd = NaN;
          let sawPositive = false;

          for (let index = 0; index < usable.length; index++) {
            const point = usable[index];
            if (point.t > positiveThreshold) {
              if (
                Number.isFinite(pauseStart) &&
                !Number.isFinite(pauseEnd)
              ) {
                pauseEnd = point.x;
                break;
              }
              sawPositive = true;
              continue;
            }

            if (sawPositive && !Number.isFinite(pauseStart)) {
              const next = usable[Math.min(usable.length - 1, index + 2)];
              if (
                next &&
                next.t <= positiveThreshold &&
                next.x - point.x >= .15
              ) {
                pauseStart = point.x;
              }
            }
          }

          if (
            Number.isFinite(pauseStart) &&
            Number.isFinite(pauseEnd) &&
            pauseEnd > pauseStart
          ) {
            if (!Number.isFinite(prePump)) {
              prePump = Math.max(0, pauseStart);
            }
            if (!Number.isFinite(prePause)) {
              prePause = Math.max(0, pauseEnd - pauseStart);
            }
          } else if (!Number.isFinite(prePump)) {
            const baselinePoints = usable.filter(point => point.x <= .6);
            const baselineSource = baselinePoints.length
              ? baselinePoints
              : usable.slice(0, Math.min(3, usable.length));
            const baseline = baselineSource.reduce(
              (sum, point) => sum + point.t,
              0
            ) / Math.max(1, baselineSource.length);
            let transition = NaN;

            for (let index = 1; index < usable.length - 2; index++) {
              const point = usable[index];
              if (point.x < .6) continue;

              const nextA = usable[index + 1];
              const nextB = usable[index + 2];
              const changed =
                Math.abs(point.t - baseline) >= .18 &&
                Math.abs(nextA.t - baseline) >= .18 &&
                Math.abs(nextB.t - baseline) >= .18;

              if (changed) {
                transition = usable[index - 1].x;
                break;
              }
            }

            prePump = Number.isFinite(transition)
              ? Math.max(0, transition)
              : 0;
          }

          if (!Number.isFinite(prePause)) {
            prePause = 0;
          }
        }
      }

      prePump = Number.isFinite(prePump)
        ? Math.max(0, Math.min(total, prePump))
        : 0;
      prePause = Number.isFinite(prePause)
        ? Math.max(0, Math.min(total - prePump, prePause))
        : 0;

      return {
        prePump,
        prePause,
        shotSeconds:Math.max(0, total - prePump - prePause)
      };
    }

    function drawArchiveStageGuides(frame, config, series) {
      if (!series.length) return;

      const chartMax = Math.max(1, xRange(series));
      const prePump = Math.max(0, Number(config.prePump) || 0);
      const prePause = Math.max(0, Number(config.prePause) || 0);
      const boundaries = [];

      if (prePump > .001 && prePump < chartMax - .001) {
        boundaries.push(prePump);
      }
      const pauseEnd = prePump + prePause;
      if (
        prePause > .001 &&
        pauseEnd > .001 &&
        pauseEnd < chartMax - .001
      ) {
        boundaries.push(pauseEnd);
      }

      if (!boundaries.length) return;

      const style = shotLineStyle('stages');
      const ctx = frame.ctx;

      ctx.save();
      ctx.strokeStyle = style.color;
      ctx.lineWidth = style.width;
      ctx.globalAlpha = style.opacity;
      ctx.setLineDash([4, 5]);

      boundaries.forEach(seconds => {
        const x =
          frame.left +
          Math.max(0, Math.min(chartMax, seconds)) /
            chartMax *
            frame.plotW;
        ctx.beginPath();
        ctx.moveTo(x, frame.top);
        ctx.lineTo(x, frame.bottom);
        ctx.stroke();
      });

      ctx.restore();
    }

    function recipeControlPointDefinitions(
      frame,
      config,
      profileName,
      series
    ) {
      if (!series.length) return [];

      const chartMax = Math.max(1, xRange(series));
      const prePump = Math.max(
        0,
        Number(config.prePump) || 0
      );
      const prePause = Math.max(
        0,
        Number(config.prePause) || 0
      );
      const mainSeconds = Math.max(
        0,
        Number(config.shotSeconds) || 0
      );
      const pauseEnd = prePump + prePause;
      const profileTotal = Math.max(
        0,
        prePump + prePause + mainSeconds
      );

      const rampSeconds = displayedProfileRampSeconds(
        config,
        profileName
      );

      const xFor = seconds =>
        frame.left +
        Math.max(0, Math.min(chartMax, seconds)) /
          chartMax *
          frame.plotW;

      const definitions = [];

      const addPressure = (
        key,
        label,
        value,
        seconds,
        color,
        dragX,
        timeField,
        timeValue,
        timeLabel
      ) => {
        const numeric = Math.max(
          0,
          Math.min(12, Number(value) || 0)
        );

        const anchorX =
          xFor(seconds);
        const handleX = anchorX;

        definitions.push({
          key,
          kind:'pressure',
          label,
          value:numeric,
          color,
          seconds,
          anchorX,
          x:handleX,
          y:yAt(frame, numeric, 0, 12),
          dragX:!!dragX,
          dragY:true,
          timeField:timeField || '',
          timeValue:Number(timeValue),
          timeLabel:timeLabel || ''
        });
      };

      const addBoundary = (
        key,
        label,
        value,
        seconds,
        color,
        field,
        yOffset,
        defaultValue
      ) => {
        const numeric = Math.max(
          0,
          Number(value) || 0
        );
        const x = xFor(seconds);
        const y = frame.bottom - yOffset;

        definitions.push({
          key,
          kind:'boundary',
          label,
          value:numeric,
          color,
          field,
          seconds,
          x,
          y,
          dragX:true,
          dragY:false,
          addState:numeric <= .05,
          defaultValue,
          hitLeft:x - 13,
          hitRight:x + 13,
          hitTop:y - 13,
          hitBottom:y + 13
        });
      };

      /*
       * The real start point remains at t = 0 for every profile.
       * Only its draggable handle is moved inward by a fixed pixel inset.
       */
      const startSeconds = 0;

      const cleanClassic =
        profileName === 'Classic' &&
        !recipeDirty &&
        !mainCurveDirty &&
        !mainCurveEditing &&
        !mainCurveDragKey;

      /*
       * Classic shows its single linked pressure handle in the middle of the
       * flat brew segment.  The first vertical drag turns the profile into a
       * Custom draft and all three pressure values become independent. Keep
       * the handle centered only during that first linked gesture; afterwards
       * place it on the real curve bend. The second and later gestures may
       * move the bend both vertically and horizontally without detaching it.
       */
      const centerClassicHandle =
        cleanClassic ||
        mainCurveClassicLinkedDrag ||
        recipeClassicLinkedDrag;
      const editablePressureBend = !centerClassicHandle;
      const mainSecondsAt = centerClassicHandle
        ? pauseEnd + mainSeconds * .5
        : pauseEnd +
          Math.min(
            mainSeconds,
            Math.max(0, rampSeconds)
          );
      const startValue = cleanClassic
        ? profileTargetAt(series, startSeconds)
        : config.prePower;
      const finishValue = cleanClassic
        ? profileTargetAt(series, profileTotal)
        : config.endPressure;

      addPressure(
        'prePower',
        'Начало',
        startValue,
        startSeconds,
        '#4bd18b',
        false,
        '',
        NaN,
        ''
      );
      addPressure(
        'mainPressure',
        'Основная',
        config.mainPressure,
        Math.min(profileTotal, mainSecondsAt),
        '#ffd166',
        editablePressureBend,
        'softInfusionTime',
        rampSeconds,
        'Набор'
      );
      addPressure(
        'endPressure',
        'Финиш',
        finishValue,
        profileTotal,
        '#54a9ff',
        false,
        '',
        NaN,
        ''
      );

      /*
       * Small handles stay near the bottom axis. Different vertical offsets
       * keep both zero-length stages individually selectable.
       */
      addBoundary(
        'prePumpTime',
        'Предсмачивание',
        prePump,
        prePump,
        '#4bd18b',
        'prePump',
        6,
        4
      );
      addBoundary(
        'prePauseTime',
        'Пауза',
        prePause,
        pauseEnd,
        '#a87ce8',
        'prePause',
        18,
        3
      );

      if (
        String(profileName || '') === 'Funnel' &&
        !recipeDirty &&
        !mainCurveDirty &&
        !mainCurveEditing &&
        !mainCurveDragKey
      ) {
        [70, 85, 115, 130, 160].forEach((seconds, index) => {
          definitions.push({
            key:'funnelStage' + index,
            kind:'boundary',
            label:'Этап Воронки',
            value:seconds,
            color:'#a87ce8',
            field:'',
            seconds,
            x:xFor(seconds),
            y:frame.bottom,
            dragX:false,
            dragY:false,
            addState:false,
            defaultValue:0,
            hitLeft:0,
            hitRight:0,
            hitTop:0,
            hitBottom:0
          });
        });
      }

      return definitions;
    }

    function drawRecipePointTooltip(frame, point) {
      const ctx = frame.ctx;
      let text = '';

      if (
        point.kind === 'boundary' ||
        point.kind === 'duration'
      ) {
        if (point.addState) {
          text =
            tr('Добавить') +
            ' ' +
            tr(point.label) +
            ' · ' +
            fmt(
              point.defaultValue,
              point.defaultValue % 1 ? 1 : 0
            ) +
            ' ' +
            tr('с');
        } else {
          text =
            tr(point.label) +
            ': ' +
            fmt(
              point.value,
              point.value % 1 ? 1 : 0
            ) +
            ' ' +
            tr('с');
        }
      } else {
        text =
          tr(point.label) +
          ': ' +
          fmt(point.value, 1) +
          ' ' +
          tr('бар');

        if (
          point.timeLabel &&
          Number.isFinite(point.timeValue)
        ) {
          text +=
            ' · ' +
            tr(point.timeLabel) +
            ' ' +
            fmt(
              point.timeValue,
              point.timeValue % 1 ? 1 : 0
            ) +
            ' ' +
            tr('с');
        }
      }

      ctx.save();
      ctx.font = chartFont(11);

      const width =
        Math.ceil(ctx.measureText(text).width) + 14;
      const height = Math.max(
        22,
        Math.round(24 * typographySettings.chart / 100)
      );

      let left = point.x - width / 2;
      left = Math.max(
        frame.left,
        Math.min(frame.right - width, left)
      );

      let top =
        point.kind === 'boundary'
          ? frame.bottom - height - 31
          : point.y - height - 13;

      if (top < frame.top) {
        top = point.y + 13;
      }

      ctx.fillStyle = 'rgba(20,23,29,.96)';
      ctx.strokeStyle = point.color;
      ctx.lineWidth = 1;
      ctx.fillRect(left, top, width, height);
      ctx.strokeRect(
        left + .5,
        top + .5,
        width - 1,
        height - 1
      );
      ctx.fillStyle = '#f3f5f7';
      ctx.textAlign = 'center';
      ctx.textBaseline = 'middle';
      ctx.fillText(
        text,
        left + width / 2,
        top + height / 2
      );
      ctx.restore();
    }

    function drawRecipeControlPoints(frame, config, profileName, series) {
      const canvas = $('recipeChart');
      recipeControlPoints = recipeControlPointDefinitions(frame, config, profileName, series);
      if (!recipeControlPoints.length) {
        if (canvas) canvas.style.cursor = 'default';
        return;
      }

      const ctx = frame.ctx;
      recipeControlPoints.forEach(point => {
        const active = recipeDragKey === point.key;
        const hovered = recipeHoverKey === point.key;
        const radius = active ? 8 : hovered ? 7 : 6;

        if (
          Number.isFinite(point.anchorX) &&
          Math.abs(point.anchorX - point.x) > .5
        ) {
          ctx.save();
          ctx.strokeStyle = point.color;
          ctx.globalAlpha =
            active || hovered
              ? .72
              : .38;
          ctx.lineWidth =
            active || hovered
              ? 1.6
              : 1.1;
          ctx.beginPath();
          ctx.moveTo(
            point.anchorX,
            point.y
          );
          ctx.lineTo(
            point.x,
            point.y
          );
          ctx.stroke();
          ctx.restore();
        }

        ctx.save();
        ctx.shadowColor = point.color;
        ctx.shadowBlur = active || hovered ? 10 : 5;
        ctx.fillStyle = active ? point.color : '#171b22';
        ctx.strokeStyle = point.color;
        ctx.lineWidth = active ? 3 : 2.2;
        ctx.beginPath();
        ctx.arc(point.x, point.y, radius, 0, Math.PI * 2);
        ctx.fill();
        ctx.stroke();

        ctx.shadowBlur = 0;
        ctx.fillStyle = active ? '#171b22' : point.color;
        ctx.beginPath();
        ctx.arc(point.x, point.y, 2.2, 0, Math.PI * 2);
        ctx.fill();
        ctx.restore();
      });

      const highlighted = recipeControlPoints.find(point => point.key === (recipeDragKey || recipeHoverKey));
      if (highlighted) drawRecipePointTooltip(frame, highlighted);

      if (canvas) canvas.style.cursor = recipeDragKey ? 'grabbing' : recipeHoverKey ? 'grab' : 'default';
    }

    function recipePointFromPointer(event) {
      const canvas = $('recipeChart');
      if (!canvas || !recipeControlPoints.length) return null;
      const rect = canvas.getBoundingClientRect();
      const x = event.clientX - rect.left;
      const y = event.clientY - rect.top;
      let nearest = null;
      let nearestDistance = Infinity;

      recipeControlPoints.forEach(point => {
        const distance = Math.hypot(point.x - x, point.y - y);
        if (distance <= 18 && distance < nearestDistance) {
          nearest = point;
          nearestDistance = distance;
        }
      });
      return nearest;
    }

    function handleRecipePointerDown(event) {
      const canvas = $('recipeChart');
      const point = recipePointFromPointer(event);
      if (!canvas || !point) return;

      recipeClassicLinkedDrag =
        point.kind === 'pressure' &&
        classicPressurePointsLinked();
      recipeDragKey = point.key;
      recipeHoverKey = point.key;
      recipeDragPointerId = event.pointerId;
      try { canvas.setPointerCapture(event.pointerId); } catch (_) {}
      event.preventDefault();
      drawRecipePreview();
    }

    function handleRecipePointerMove(event) {
      const canvas = $('recipeChart');
      if (!canvas) return;

      if (recipeDragKey && recipeDragPointerId === event.pointerId) {
        const rect = canvas.getBoundingClientRect();
        const top = 22;
        const bottom = rect.height - 25;
        const plotHeight = Math.max(1, bottom - top);
        const y = Math.max(top, Math.min(bottom, event.clientY - rect.top));
        const raw = (bottom - y) / plotHeight * 12;
        const value = Math.max(0, Math.min(12, Math.round(raw * 10) / 10));
        const pressureKey = ['prePower', 'mainPressure', 'endPressure'].includes(recipeDragKey);
        const point = recipeControlPoints.find(item => item.key === recipeDragKey);

        if (pressureKey && classicPressurePointsLinked()) {
          if (setClassicPressureInputs(value)) recipeFieldChanged();
        } else {
          const input = $(recipeDragKey);
          if (input && Math.abs((Number(input.value) || 0) - value) >= 0.049) {
            input.value = value.toFixed(1);
            recipeFieldChanged();
          }
          if (recipeDragKey === 'mainPressure' && point?.dragX) {
            const left = 54;
            const right = rect.width - 16;
            const plotWidth = Math.max(1, right - left);
            const config = recipeConfigFromInputs();
            const total = Math.max(1, xRange(buildProfileSeries(config, 'Custom')));
            const seconds = Math.max(0, Math.min(total, (event.clientX - rect.left - left) / plotWidth * total));
            const pauseEnd = Math.max(0, Number(config.prePump) || 0) + Math.max(0, Number(config.prePause) || 0);
            const ramp = Math.max(0, Math.min(Number(config.shotSeconds) || 0, seconds - pauseEnd));
            const timeInput = $('softInfusionTime');
            if (timeInput && Math.abs((Number(timeInput.value) || 0) - ramp) >= .049) {
              timeInput.value = ramp.toFixed(1);
              recipeFieldChanged();
            }
          }
        }
        event.preventDefault();
        return;
      }

      const point = recipePointFromPointer(event);
      const nextHover = point ? point.key : '';
      if (nextHover !== recipeHoverKey) {
        recipeHoverKey = nextHover;
        drawRecipePreview();
      }
    }

    function finishRecipePointer(event) {
      const canvas = $('recipeChart');
      if (recipeDragKey && (recipeDragPointerId === null || recipeDragPointerId === event.pointerId)) {
        try {
          if (canvas && canvas.hasPointerCapture(event.pointerId)) canvas.releasePointerCapture(event.pointerId);
        } catch (_) {}
        recipeDragKey = '';
        recipeClassicLinkedDrag = false;
        recipeDragPointerId = null;
        const point = recipePointFromPointer(event);
        recipeHoverKey = point ? point.key : '';
        drawRecipePreview();
      }
    }

    function drawRecipePreview() {
      activeChartFontZone = 'global';

      const canvas = $('recipeChart');
      if (!canvas || !canvas.isConnected || canvas.getBoundingClientRect().width < 20) return;
      const editedConfig = recipeConfigFromInputs();
      const editedProfile = recipeDirty ? 'Custom' : (recipeBaseProfile || currentProfileName || 'Custom');
      const editedSeries = buildProfileSeries(editedConfig, editedProfile);
      const baselineSeries = recipeBaselineCfg ? buildProfileSeries(recipeBaselineCfg, recipeBaseProfile || currentProfileName || 'Custom') : [];
      const frame = canvasFrame('recipeChart', { left: 54, right: 16, top: 22, bottom: 25 });

      if (advancedPhaseDraft) drawAdvancedPhaseBands(frame);
      else drawRecipeBands(
        frame,
        editedConfig,
        editedSeries,
        displayedAdvancedPhases(editedProfile, editedConfig)
      );
      drawRecipePressureAxes(frame, editedSeries);
      if (recipeDirty && baselineSeries.length) plotLine(frame, baselineSeries, 't', '#707986', 0, 12, 2.0, false);
      plotLine(frame, editedSeries, 't', '#ffd166', 0, 12, 2.8, true);
      drawRecipeControlPoints(frame, editedConfig, editedProfile, editedSeries);
      updateRecipeDescription();
    }

    function drawAdvancedPhaseBands(frame) {
      if (!advancedPhaseDraft) return;
      const ctx = frame.ctx;
      const total = Math.max(.5, phaseDraftTotalSeconds());
      let elapsed = 0;
      advancedPhaseDraft.forEach((phase, index) => {
        const duration = Math.max(.5, phaseNumber(phase.duration));
        const left = frame.left + elapsed / total * frame.plotW;
        const right = frame.left + (elapsed + duration) / total * frame.plotW;
        const selected = index === advancedPhaseSelected;
        const pause = phase.type === 'pause';
        const preinfusion = phase.type === 'preinfusion';
        ctx.save();
        ctx.fillStyle = selected
          ? 'rgba(255,209,102,.12)'
          : pause
            ? 'rgba(84,169,255,.075)'
            : preinfusion
              ? 'rgba(75,209,139,.06)'
              : index % 2
                ? 'rgba(225,139,72,.035)'
                : 'rgba(255,255,255,.018)';
        ctx.fillRect(left, frame.top, Math.max(1, right-left), frame.plotH);

        const phaseLabel = phaseDisplayName(phase, index, advancedPhaseDraft);
        const durationLabel = fmt(duration, duration % 1 ? 1 : 0) + ' ' + tr('с');
        const fullLabel = phaseLabel + ' · ' + durationLabel;
        const availableWidth = Math.max(0, right - left - 8);
        ctx.globalAlpha = selected ? 1 : .86;
        ctx.fillStyle = selected ? '#ffd166' : '#cbd3dc';
        ctx.font = chartFont(9, selected ? 700 : 600);
        ctx.textAlign = 'center';
        ctx.textBaseline = 'top';
        const shortLabel = phase.type === 'preinfusion'
          ? tr('Смачив.')
          : phase.type === 'pause'
            ? tr('Пауза')
            : tr('Подача');
        const label = [
          fullLabel,
          shortLabel + ' · ' + durationLabel,
          shortLabel,
          durationLabel
        ].find(value => ctx.measureText(value).width <= availableWidth) || '';
        if (label) {
          ctx.fillText(label, left + (right - left) / 2, 7);
        }

        const arrowY = Math.max(25, frame.top - 13);
        const width = right - left;
        const innerLeft = Math.min(right, left + Math.min(8, width * .22));
        const innerRight = Math.max(left, right - Math.min(8, width * .22));
        const arrow = Math.min(4, Math.max(2, width * .08));
        ctx.strokeStyle = selected ? '#ffd166' : 'rgba(151,162,175,.70)';
        ctx.lineWidth = selected ? 1.4 : 1;
        ctx.globalAlpha = selected ? 1 : .86;
        ctx.beginPath();
        ctx.moveTo(left + .5, arrowY - 7);
        ctx.lineTo(left + .5, arrowY + 7);
        ctx.moveTo(right - .5, arrowY - 7);
        ctx.lineTo(right - .5, arrowY + 7);
        ctx.stroke();
        if (width >= 14) {
          ctx.beginPath();
          ctx.moveTo(innerLeft, arrowY);
          ctx.lineTo(innerRight, arrowY);
          ctx.moveTo(innerLeft, arrowY);
          ctx.lineTo(innerLeft + arrow, arrowY - arrow);
          ctx.moveTo(innerLeft, arrowY);
          ctx.lineTo(innerLeft + arrow, arrowY + arrow);
          ctx.moveTo(innerRight, arrowY);
          ctx.lineTo(innerRight - arrow, arrowY - arrow);
          ctx.moveTo(innerRight, arrowY);
          ctx.lineTo(innerRight - arrow, arrowY + arrow);
          ctx.stroke();
        }

        if (index > 0) {
          ctx.strokeStyle = '#8aa0b6';
          ctx.lineWidth = 1.25;
          ctx.globalAlpha = .72;
          ctx.setLineDash([5,4]);
          ctx.beginPath();
          ctx.moveTo(left, frame.top);
          ctx.lineTo(left, frame.bottom);
          ctx.stroke();

          ctx.setLineDash([]);
          ctx.globalAlpha = .92;
          ctx.fillStyle = selected ? '#ffd166' : '#aab7c4';
          ctx.font = chartFont(9, 650);
          ctx.textAlign = 'center';
          ctx.textBaseline = 'top';
          ctx.fillText(fmt(elapsed, elapsed % 1 ? 1 : 0) + ' c', left, frame.top + 3);
        }
        ctx.restore();
        elapsed += duration;
      });
    }

    function drawAdvancedPhaseControlPoints(frame) {
      const canvas = $('chart');
      mainCurveControlPoints = [];
      const phases = normalizeAdvancedPhases(advancedPhaseDraft);
      if (!phases) return;

      const ctx = frame.ctx;
      const total = Math.max(.5,phaseDraftTotalSeconds(phases));
      let elapsed = 0;

      phases.forEach((phase,index) => {
        const duration = Math.max(.5,phaseNumber(phase.duration));
        const startX = frame.left + elapsed / total * frame.plotW;
        const endX = frame.left + (elapsed + duration) / total * frame.plotW;

        if (isAdvancedPressurePhase(phase)) {
          const previousLinked = isAdvancedPressurePhase(phases[index-1]);
          const nextLinked = isAdvancedPressurePhase(phases[index+1]);
          const flatPreinfusion =
            phase.type === 'preinfusion' &&
            !advancedPreinfusionRampMode;
          const points = [];

          if (flatPreinfusion) {
            points.push({
              field:'both',
              x:(startX+endX)/2,
              value:phaseNumber(phase.start),
              nextPhaseIndex:nextLinked ? index+1 : -1,
              label:'Давление предсмачивания'
            });
          } else {
            if (!previousLinked) {
              points.push({
                field:'start',
                x:startX,
                value:phaseNumber(phase.start),
                nextPhaseIndex:-1,
                label:'Начало'
              });
            }
            points.push({
              field:'end',
              x:endX,
              value:phaseNumber(phase.end),
              nextPhaseIndex:nextLinked ? index+1 : -1,
              label:nextLinked ? 'Общий переход' : 'Финиш'
            });
          }

          points.forEach(point => {
            const y = frame.bottom - point.value / 12 * frame.plotH;
            const key = 'ap:' + index + ':' + point.field;
            const active = mainCurveDragKey === key;
            const hovered = mainCurveHoverKey === key;
            mainCurveControlPoints.push({
              key,
              kind:'pressure',
              phaseIndex:index,
              phaseField:point.field,
              nextPhaseIndex:point.nextPhaseIndex,
              label:point.label,
              x:point.x,
              y,
              value:point.value,
              color:'#ffd166'
            });

            ctx.save();
            ctx.shadowColor = '#ffd166';
            ctx.shadowBlur = active || hovered ? 11 : 4;
            ctx.fillStyle = active ? '#ffd166' : '#171b22';
            ctx.strokeStyle = '#ffd166';
            ctx.lineWidth = active ? 3 : 2;
            ctx.beginPath();
            ctx.arc(point.x,y,active?8:hovered?7:6,0,Math.PI*2);
            ctx.fill();
            ctx.stroke();
            ctx.shadowBlur = 0;
            ctx.fillStyle = active ? '#171b22' : '#ffd166';
            ctx.beginPath();
            ctx.arc(point.x,y,2.1,0,Math.PI*2);
            ctx.fill();

            const pressureText = fmt(point.value,1) + ' ' + tr('бар');
            ctx.font = chartFont(9,700);
            ctx.textAlign = 'center';
            ctx.textBaseline = 'middle';
            const textWidth = Math.ceil(ctx.measureText(pressureText).width);
            const pillWidth = textWidth + 10;
            const pillHeight = 17;
            const radius = active ? 8 : hovered ? 7 : 6;
            const aboveTop = y - radius - pillHeight - 5;
            const pillTop = aboveTop >= frame.top + 2
              ? aboveTop
              : Math.min(frame.bottom-pillHeight-2,y+radius+5);
            const pillLeft = Math.max(
              frame.left,
              Math.min(frame.right-pillWidth,point.x-pillWidth/2)
            );
            ctx.shadowBlur = 0;
            ctx.globalAlpha = active || hovered ? 1 : .88;
            ctx.fillStyle = active ? '#ffd166' : 'rgba(23,27,34,.92)';
            ctx.strokeStyle = '#ffd166';
            ctx.lineWidth = active ? 1.6 : 1;
            if (ctx.roundRect) {
              ctx.beginPath();
              ctx.roundRect(pillLeft,pillTop,pillWidth,pillHeight,5);
              ctx.fill();
              ctx.stroke();
            } else {
              ctx.fillRect(pillLeft,pillTop,pillWidth,pillHeight);
              ctx.strokeRect(pillLeft,pillTop,pillWidth,pillHeight);
            }
            ctx.globalAlpha = 1;
            ctx.fillStyle = active ? '#171b22' : '#ffd166';
            ctx.fillText(
              pressureText,
              pillLeft+pillWidth/2,
              pillTop+pillHeight/2+.2
            );
            ctx.restore();
          });
        }

        const key = 'ab:' + index;
        const stageGuide = shotLineStyle('stages');
        const active = mainCurveDragKey === key;
        const selected = advancedPhaseSelected === index;
        mainCurveControlPoints.push({
          key,
          kind:'duration',
          phaseIndex:index,
          x:endX,
          y:(frame.top+frame.bottom)/2,
          value:duration,
          color:selected ? '#ffd166' : '#8aa0b6',
          hitLeft:endX-12,
          hitRight:endX+12,
          hitTop:frame.top,
          hitBottom:frame.bottom
        });
        ctx.save();
        ctx.strokeStyle = active || selected ? '#ffd166' : stageGuide.color;
        ctx.lineWidth = active || selected
          ? Math.max(2,stageGuide.width)
          : stageGuide.width;
        ctx.globalAlpha = active || selected
          ? .9
          : Math.max(.35,stageGuide.opacity);
        ctx.setLineDash([4,5]);
        ctx.beginPath();
        ctx.moveTo(endX,frame.top);
        ctx.lineTo(endX,frame.bottom);
        ctx.stroke();
        ctx.restore();

        elapsed += duration;
      });

      const highlighted = mainCurveControlPoints.find(
        point =>
          point.kind === 'pressure' &&
          point.key === (mainCurveDragKey || mainCurveHoverKey)
      );
      if (highlighted) {
        drawRecipePointTooltip(frame, highlighted);
      }

      if (canvas) {
        const point = mainCurveControlPoints.find(item => item.key === (mainCurveDragKey || mainCurveHoverKey));
        canvas.style.cursor = !point ? 'default' :
          mainCurveDragKey ? 'grabbing' :
          point.kind === 'duration' ? 'ew-resize' : 'ns-resize';
      }
    }

    function drawMainCurveControlPoints(frame, series) {
      const canvas = $('chart');

      if (advancedPhaseDraft) {
        drawAdvancedPhaseControlPoints(frame);
        return;
      }

      mainCurveControlPoints =
        recipeControlPointDefinitions(
          frame,
          profileDisplayConfig(),
          (
            recipeDirty ||
            mainCurveDirty ||
            mainCurveEditing ||
            mainCurveDragKey
          ) ? 'Custom' : (currentProfileName || 'Custom'),
          series
        )
        .filter(
          point =>
            point.kind === 'pressure' ||
            point.kind === 'boundary'
        );

      const ctx = frame.ctx;
      const enabled =
        mainCurveCanEdit() ||
        mainCurveDirty ||
        mainCurveDragKey;

      mainCurveControlPoints.forEach(point => {
        const active =
          mainCurveDragKey === point.key;
        const hovered =
          mainCurveHoverKey === point.key;

        if (point.kind === 'duration') {
          const pillWidth = 43;
          const pillHeight = 18;
          const pillLeft =
            point.x - pillWidth - 5;
          const pillTop =
            point.y - pillHeight / 2;

          const stageGuide = shotLineStyle('stages');
          ctx.save();
          ctx.globalAlpha = enabled ? 1 : .55;
          ctx.strokeStyle =
            active || hovered
              ? point.color
              : stageGuide.color;
          ctx.lineWidth =
            active
              ? Math.max(2, stageGuide.width)
              : hovered
                ? Math.max(1.5, stageGuide.width)
                : stageGuide.width;
          ctx.setLineDash(
            active || hovered
              ? [5, 4]
              : [4, 5]
          );
          ctx.globalAlpha =
            active || hovered
              ? .82
              : stageGuide.opacity;
          ctx.beginPath();
          ctx.moveTo(point.x, frame.top);
          ctx.lineTo(point.x, frame.bottom);
          ctx.stroke();

          ctx.setLineDash([]);
          ctx.globalAlpha = enabled ? 1 : .55;
          ctx.fillStyle =
            active || hovered
              ? point.color
              : '#171b22';
          ctx.strokeStyle = point.color;
          ctx.lineWidth =
            active ? 2.2 : 1.4;

          if (ctx.roundRect) {
            ctx.beginPath();
            ctx.roundRect(
              pillLeft,
              pillTop,
              pillWidth,
              pillHeight,
              6
            );
            ctx.fill();
            ctx.stroke();
          } else {
            ctx.fillRect(
              pillLeft,
              pillTop,
              pillWidth,
              pillHeight
            );
            ctx.strokeRect(
              pillLeft,
              pillTop,
              pillWidth,
              pillHeight
            );
          }

          ctx.beginPath();
          ctx.moveTo(
            pillLeft + pillWidth,
            point.y
          );
          ctx.lineTo(
            point.x,
            point.y
          );
          ctx.stroke();

          ctx.font = chartFont(9);
          ctx.fillStyle =
            active || hovered
              ? '#10151a'
              : point.color;
          ctx.textAlign = 'center';
          ctx.textBaseline = 'middle';
          ctx.fillText(
            fmt(
              point.value,
              point.value % 1 ? 1 : 0
            ) +
              ' ' +
              tr('с'),
            pillLeft + pillWidth / 2,
            point.y
          );
          ctx.restore();
          return;
        }

        if (point.kind === 'boundary') {
          const stageGuide = shotLineStyle('stages');
          ctx.save();
          ctx.strokeStyle = stageGuide.color;
          ctx.lineWidth = stageGuide.width;
          ctx.setLineDash([4, 5]);
          ctx.globalAlpha = stageGuide.opacity;
          ctx.beginPath();
          ctx.moveTo(point.x, frame.top);
          ctx.lineTo(point.x, frame.bottom);
          ctx.stroke();
          ctx.restore();
          return;
        }

        const radius = active
          ? 8
          : hovered
            ? 7
            : 6;

        if (
          Number.isFinite(point.anchorX) &&
          Math.abs(point.anchorX - point.x) > .5
        ) {
          ctx.save();
          ctx.strokeStyle = point.color;
          ctx.globalAlpha =
            active || hovered
              ? .78
              : .42;
          ctx.lineWidth =
            active || hovered
              ? 1.7
              : 1.15;
          ctx.beginPath();
          ctx.moveTo(
            point.anchorX,
            point.y
          );
          ctx.lineTo(
            point.x,
            point.y
          );
          ctx.stroke();
          ctx.restore();
        }

        ctx.save();
        ctx.globalAlpha = enabled ? 1 : .55;
        ctx.shadowColor = point.color;
        ctx.shadowBlur =
          active || hovered
            ? 11
            : 5;
        ctx.fillStyle =
          active
            ? point.color
            : '#171b22';
        ctx.strokeStyle = point.color;
        ctx.lineWidth = active ? 3 : 2.2;
        ctx.beginPath();
        ctx.arc(
          point.x,
          point.y,
          radius,
          0,
          Math.PI * 2
        );
        ctx.fill();
        ctx.stroke();
        ctx.shadowBlur = 0;
        ctx.fillStyle =
          active
            ? '#171b22'
            : point.color;
        ctx.beginPath();
        ctx.arc(
          point.x,
          point.y,
          2.2,
          0,
          Math.PI * 2
        );
        ctx.fill();

        ctx.font = chartFont(9);
        ctx.textAlign = 'center';
        ctx.textBaseline = 'bottom';
        ctx.fillStyle = point.color;

        const label =
          fmt(point.value, 1);
        const labelWidth =
          ctx.measureText(label).width;
        const labelPadding = 5;
        const labelX = Math.max(
          frame.left +
            labelWidth / 2 +
            labelPadding,
          Math.min(
            frame.right -
              labelWidth / 2 -
              labelPadding,
            point.x
          )
        );
        const labelY = Math.max(
          frame.top + 24,
          point.y - radius - 4
        );

        ctx.fillText(
          label,
          labelX,
          labelY
        );
        ctx.restore();
      });

      const highlighted =
        mainCurveControlPoints.find(
          point =>
            point.key ===
            (mainCurveDragKey || mainCurveHoverKey)
        );

      if (highlighted) {
        drawRecipePointTooltip(
          frame,
          highlighted
        );
      }

      if (canvas) {
        const activePoint =
          mainCurveControlPoints.find(
            point =>
              point.key === mainCurveDragKey
          );
        const hoverPoint =
          mainCurveControlPoints.find(
            point =>
              point.key === mainCurveHoverKey
          );
        const point =
          activePoint || hoverPoint;

        canvas.style.cursor = !enabled
          ? 'default'
          : activePoint
            ? 'grabbing'
            : (
                point?.kind === 'boundary' ||
                point?.kind === 'duration'
              )
              ? 'ew-resize'
              : point?.dragX
                ? 'move'
                : point
                  ? 'ns-resize'
                  : 'default';
      }
    }

    function mainCurvePointFromPointer(event) {
      const canvas = $('chart');

      if (!canvas || !mainCurveControlPoints.length) {
        return null;
      }

      const rect = canvas.getBoundingClientRect();
      const x = event.clientX - rect.left;
      const y = event.clientY - rect.top;

      let nearest = null;
      let distance = Infinity;

      mainCurveControlPoints
        .filter(point => point.kind === 'pressure')
        .forEach(point => {
          const hitRadius =
            point.key === 'endPressure'
              ? 30
              : point.key === 'mainPressure'
                ? 24
                : 22;
          const next = Math.hypot(
            point.x - x,
            point.y - y
          );

          if (
            next <= hitRadius &&
            next < distance
          ) {
            nearest = point;
            distance = next;
          }
        });

      if (nearest) return nearest;

      mainCurveControlPoints
        .filter(
          point => point.kind === 'duration'
        )
        .forEach(point => {
          const inside =
            x >= point.hitLeft &&
            x <= point.hitRight &&
            y >= point.hitTop &&
            y <= point.hitBottom;

          if (!inside) return;

          const centerX =
            (
              point.hitLeft +
              point.hitRight
            ) / 2;
          const centerY =
            (
              point.hitTop +
              point.hitBottom
            ) / 2;
          const next = Math.hypot(
            centerX - x,
            centerY - y
          );

          if (next < distance) {
            nearest = point;
            distance = next;
          }
        });

      return nearest;
    }

    function mainCurvePressureFromPointer(event) {
      const canvas = $('chart');
      if (!canvas) return 0;
      const rect = canvas.getBoundingClientRect();
      const top = 18;
      const bottom = rect.height - 24;
      const plotHeight = Math.max(1, bottom - top);
      const y = Math.max(top, Math.min(bottom, event.clientY - rect.top));
      return Math.max(0, Math.min(12, Math.round(((bottom - y) / plotHeight * 12) * 10) / 10));
    }

    function setMainCurveInput(id, value, min, max) {
      const input = $(id);
      if (!input) return false;
      const numeric = Math.max(min, Math.min(max, Number(value)));
      const rounded = Math.round(numeric * 10) / 10;
      if (Math.abs((Number(input.value) || 0) - rounded) < .049) return false;
      input.value = rounded.toFixed(1);
      return true;
    }

    function classicPressurePointsLinked() {
      const basedOnClassic = String(
        recipeBaseProfile ||
        currentProfileName ||
        ''
      ) === 'Classic';

      /*
       * A clean Classic keeps all pressure points linked. The gesture that
       * first changes it also stays linked until pointer-up, so one smooth
       * drag moves the complete flat line. Afterwards it is a Custom draft
       * and the three pressure points can be edited independently.
       */
      return basedOnClassic && (
        (!recipeDirty && !mainCurveDirty) ||
        mainCurveClassicLinkedDrag ||
        recipeClassicLinkedDrag
      );
    }

    function setClassicPressureInputs(value) {
      let changed = false;
      ['prePower', 'mainPressure', 'endPressure'].forEach(id => {
        changed = setMainCurveInput(id, value, 0, 12) || changed;
      });
      return changed;
    }

    function commitMainCurveChange() {
      recipeFieldChanged();
      mainCurveDirty = true;
      mainCurveEditing = true;

      if (activeUserProfileId) {
        activeUserProfileId = '';
        renderUserProfiles();
      }

      updateMainCurveEditor();
    }

    function handleMainChartDoubleClick(event) {
      if (profilePhaseEditMode) return;
      if (!mainCurveCanEdit()) return;
      const canvas = $('chart');
      if (!canvas) return;

      const rect = canvas.getBoundingClientRect();
      const left = 36;
      const right = rect.width - 60;
      const plotWidth = Math.max(1, right - left);
      const chartTotal = xRange(chartSeries());
      const seconds = Math.max(0, Math.min(chartTotal, (event.clientX - rect.left - left) / plotWidth * chartTotal));
      const config = recipeConfigFromInputs();
      const pauseEnd = Math.max(0, Number(config.prePump) || 0) + Math.max(0, Number(config.prePause) || 0);
      const shotSeconds = Math.max(0, Number(config.shotSeconds) || 0);
      const brewElapsed = seconds - pauseEnd;

      if (brewElapsed < 0 || brewElapsed > shotSeconds) {
        toast(tr('Двойной клик доступен в стадии основного пролива'), true);
        return;
      }
      if (!beginMainCurveEditSession()) return;

      let changed = false;
      const pressure = mainCurvePressureFromPointer(event);
      if (classicPressurePointsLinked()) {
        changed = setClassicPressureInputs(pressure) || changed;
      } else {
        changed = setMainCurveInput('mainPressure', pressure, 0, 12) || changed;
      }
      changed = setMainCurveInput('softInfusionTime', brewElapsed, 0, shotSeconds) || changed;

      if (changed) {
        mainCurveHoverKey = 'mainPressure';
        commitMainCurveChange();
      }
      event.preventDefault();
    }

    function handleMainChartPointerDown(event) {
      const point = mainCurvePointFromPointer(event);

      if (!point || !mainCurveCanEdit()) {
        handleChartPointer(event);
        return;
      }

      const pointKey = point.key;
      const linkClassicForThisDrag =
        point.kind === 'pressure' &&
        classicPressurePointsLinked();

      if (!beginMainCurveEditSession()) return;

      const canvas = $('chart');
      const rect = canvas.getBoundingClientRect();
      const config = recipeConfigFromInputs();
      const profileTotal = advancedPhaseDraft
        ? Math.max(1,phaseDraftTotalSeconds(advancedPhaseDraft))
        : Math.max(
            1,
            Math.max(0, Number(config.prePump) || 0) +
            Math.max(0, Number(config.prePause) || 0) +
            Math.max(0, Number(config.shotSeconds) || 0)
          );

      mainCurveDragKey = pointKey;
      mainCurveHoverKey = pointKey;
      mainCurveDragPointerId = event.pointerId;
      mainCurveDragStartX = event.clientX;
      mainCurveDragStartY = event.clientY;
      mainCurveDragMoved = false;
      mainCurveDragStartConfig = advancedPhaseDraft
        ? { phases:cloneAdvancedPhases(advancedPhaseDraft) }
        : Object.assign({},config);
      mainCurveClassicLinkedDrag =
        linkClassicForThisDrag;
      mainCurveDragSecondsPerPixel = Math.max(
        .02,
        profileTotal /
          Math.max(1, rect.width - 96)
      );

      try {
        canvas.setPointerCapture(event.pointerId);
      } catch (_) {}

      event.preventDefault();
    }

    function handleMainChartPointerMove(event) {
      const canvas = $('chart');
      if (!canvas) return;

      if (
        mainCurveDragKey &&
        mainCurveDragPointerId === event.pointerId &&
        mainCurveDragStartConfig
      ) {
        const dx = event.clientX - mainCurveDragStartX;
        const dy = event.clientY - mainCurveDragStartY;

        if (
          !mainCurveDragMoved &&
          Math.hypot(dx, dy) > 4
        ) {
          mainCurveDragMoved = true;
        }

        const point = mainCurveControlPoints.find(
          item => item.key === mainCurveDragKey
        );
        const start = mainCurveDragStartConfig;
        const deltaSeconds =
          dx * mainCurveDragSecondsPerPixel;
        const pressure =
          mainCurvePressureFromPointer(event);
        let changed = false;

        if (advancedPhaseDraft && Number.isInteger(point?.phaseIndex)) {
          const phaseIndex = point.phaseIndex;
          const startPhases = start?.phases;
          if (!Array.isArray(startPhases) || !startPhases[phaseIndex]) {
            event.preventDefault();
            return;
          }

          if (point.kind === 'pressure') {
            const phase = advancedPhaseDraft[phaseIndex];
            if (phase?.type !== 'pause') {
              const next = Math.max(0,Math.min(12,pressure));
              if (point.phaseField === 'both') {
                if (
                  Math.abs(phaseNumber(phase.start)-next) > .049 ||
                  Math.abs(phaseNumber(phase.end)-next) > .049
                ) {
                  phase.start = next;
                  phase.end = next;
                  changed = true;
                }
              } else if (
                point.phaseField === 'start' ||
                point.phaseField === 'end'
              ) {
                if (Math.abs(phaseNumber(phase[point.phaseField])-next) > .049) {
                  phase[point.phaseField] = next;
                  changed = true;
                }
              }

              const linkedIndex = Number(point.nextPhaseIndex);
              const linkedPhase = Number.isInteger(linkedIndex)
                ? advancedPhaseDraft[linkedIndex]
                : null;
              if (
                linkedPhase &&
                isAdvancedPressurePhase(linkedPhase) &&
                Math.abs(phaseNumber(linkedPhase.start)-next) > .049
              ) {
                linkedPhase.start = next;
                changed = true;
              }
            }
          } else if (point.kind === 'duration') {
            const original = Math.max(
              .5,
              phaseNumber(startPhases[phaseIndex].duration)
            );
            const otherTotal = startPhases.reduce(
              (sum,phase,index) => index === phaseIndex
                ? sum
                : sum + Math.max(.5,phaseNumber(phase.duration)),
              0
            );
            const maximum = Math.max(.5,240-otherTotal);
            const next = Math.max(
              .5,
              Math.min(maximum,original+deltaSeconds)
            );
            const rounded = Math.round(next*10)/10;
            if (
              Math.abs(
                phaseNumber(advancedPhaseDraft[phaseIndex].duration)-rounded
              ) > .049
            ) {
              advancedPhaseDraft[phaseIndex].duration = rounded;
              changed = true;
            }
          }

          if (changed) {
            advancedPhaseSelected = phaseIndex;
            recipeDirty = true;
            mainCurveDirty = true;
            renderAdvancedPhaseEditor();
            updateShotScenario();
            requestAnimationFrame(draw);
          }
          event.preventDefault();
          return;
        }

        if (
          point?.kind === 'boundary' ||
          point?.kind === 'duration'
        ) {
          if (point.field === 'prePump') {
            changed =
              setMainCurveInput(
                'prePump',
                Number(start.prePump) +
                  deltaSeconds,
                0,
                20
              ) || changed;
          } else if (
            point.field === 'prePause'
          ) {
            changed =
              setMainCurveInput(
                'prePause',
                Number(start.prePause) +
                  deltaSeconds,
                0,
                20
              ) || changed;
          } else if (
            point.field === 'shotSeconds'
          ) {
            changed =
              setMainCurveInput(
                'shotSeconds',
                Number(start.shotSeconds) +
                  deltaSeconds,
                1,
                120
              ) || changed;
          }
        } else if (
          ['prePower', 'mainPressure', 'endPressure'].includes(mainCurveDragKey)
        ) {
          if (classicPressurePointsLinked()) {
            changed = setClassicPressureInputs(pressure) || changed;
          } else {
            changed =
              setMainCurveInput(
                mainCurveDragKey,
                pressure,
                0,
                12
              ) || changed;
          }

          if (
            mainCurveDragKey === 'mainPressure' &&
            point?.dragX
          ) {
            changed =
              setMainCurveInput(
                'softInfusionTime',
                Number(start.softInfusionTime) +
                  deltaSeconds,
                0,
                Math.max(0, Number(start.shotSeconds) || 0)
              ) || changed;
          }

        }

        if (changed) {
          commitMainCurveChange();
        }

        event.preventDefault();
        return;
      }

      const point = mainCurvePointFromPointer(event);
      const next =
        point && mainCurveCanEdit()
          ? point.key
          : '';

      if (next) {
        chartHoverIndex = -1;
        hideChartTooltip();

        if (next !== mainCurveHoverKey) {
          mainCurveHoverKey = next;
          draw();
        }

        return;
      }

      if (mainCurveHoverKey) {
        mainCurveHoverKey = '';
      }

      handleChartPointer(event);
    }

    function finishMainCurvePointer(event) {
      const canvas = $('chart');

      if (
        mainCurveDragKey &&
        (
          mainCurveDragPointerId === null ||
          mainCurveDragPointerId === event.pointerId
        )
      ) {
        const finishedKey = mainCurveDragKey;
        const start = mainCurveDragStartConfig;
        const wasMoved = mainCurveDragMoved;
        const wasCancelled =
          event.type === 'pointercancel';

        if (
          !wasCancelled &&
          !wasMoved &&
          start
        ) {
          if (
            finishedKey === 'prePumpTime' &&
            Math.max(
              0,
              Number(start.prePump) || 0
            ) <= .05
          ) {
            if (
              setMainCurveInput(
                'prePump',
                4,
                0,
                20
              )
            ) {
              commitMainCurveChange();
            }
          } else if (
            finishedKey === 'prePauseTime' &&
            Math.max(
              0,
              Number(start.prePause) || 0
            ) <= .05
          ) {
            if (
              setMainCurveInput(
                'prePause',
                3,
                0,
                20
              )
            ) {
              commitMainCurveChange();
            }
          }
        }

        try {
          if (
            canvas &&
            canvas.hasPointerCapture(event.pointerId)
          ) {
            canvas.releasePointerCapture(
              event.pointerId
            );
          }
        } catch (_) {}

        mainCurveDragKey = '';
        mainCurveDragPointerId = null;
        mainCurveDragStartConfig = null;
        mainCurveDragMoved = false;
        mainCurveClassicLinkedDrag = false;

        if (!mainCurveDirty) {
          mainCurveEditing = false;
          mainCurveBaselineCfg = null;
          mainCurveBaselineDirty = false;
        }

        requestAnimationFrame(() => {
          draw();

          const point =
            mainCurvePointFromPointer(event);

          mainCurveHoverKey =
            point && mainCurveCanEdit()
              ? point.key
              : '';

          updateMainCurveEditor();
        });
      }
    }

    function draw() {
      activeChartFontZone = 'shot';

      const canvas = $('chart');
      if (!canvas || !canvas.isConnected || canvas.getBoundingClientRect().width < 20) {
        updateChartControls();
        return;
      }
      const series = chartSeries();
      const hasFact = seriesContainsFact(series);
      const frame = canvasFrame('chart', { left: 54, right: 60, top: 34, bottom: 24 });
      const profileView = chartMode !== 'archive';
      const archiveStages = profileView
        ? null
        : archiveStageConfig(archiveSelectedSummary, series);

      if (profileView) {
        if (advancedPhaseDraft) drawAdvancedPhaseBands(frame);
        else {
          const displayConfig = profileDisplayConfig();
          drawRecipeBands(
            frame,
            displayConfig,
            series,
            displayedAdvancedPhases(currentProfileName, displayConfig)
          );
        }
      } else if (archiveStages) {
        drawRecipeBands(frame, archiveStages, series);
      }
      drawPressureAxes(frame, series);
      if (archiveStages) {
        drawArchiveStageGuides(frame, archiveStages, series);
      }

      const profileLine =
        shotLineStyle('profile');

      if (shotLineVisible('profile')) {
        plotLine(
          frame,
          series,
          't',
          profileLine.color,
          0,
          12,
          profileLine.width,
          true,
          profileLine.opacity
        );
      }

      if (hasFact) {
        const pumpLine =
          shotLineStyle('pump');
        const flowLine =
          shotLineStyle('flow');
        const pressureLine =
          shotLineStyle('pressure');

        if (shotLineVisible('pump')) {
          plotLine(frame, series, 'u', pumpLine.color, 0, 100, pumpLine.width, false, pumpLine.opacity);
        }
        if (shotLineVisible('flow')) {
          plotLine(frame, series, 'f', flowLine.color, 0, 6, flowLine.width, false, flowLine.opacity);
        }
        if (shotLineVisible('pressure')) {
          plotLine(frame, series, 'p', pressureLine.color, 0, 12, pressureLine.width, false, pressureLine.opacity);
        }
      }

      if (profileView && !shotRunning && profilePhaseEditMode) {
        const editingFocus = !!(mainCurveDragKey || mainCurveHoverKey);
        if (editingFocus) hideChartTooltip();
        else drawChartHover(frame, series);
        drawMainCurveControlPoints(frame, series);
      } else {
        mainCurveControlPoints = [];
        canvas.style.cursor = 'default';
        drawChartHover(frame, series);
      }

      updateChartControls();
    }

    function setTemperatureDetails(open) {
      const expanded = !!open;
      homeTemperatureMode = expanded ? 'graph' : 'state';

      const card = $('temperatureDetailsCard');
      const trigger = $('temperatureMetricOpen');
      if (card) card.hidden = !expanded;
      if (trigger) {
        trigger.classList.toggle('expanded', expanded);
        trigger.setAttribute('aria-expanded', expanded ? 'true' : 'false');
        trigger.title = tr(expanded ? 'Скрыть график температуры' : 'Показать график температуры');
      }

      if (expanded) {
        setTemperatureWindow(tempWindowSeconds);
        requestAnimationFrame(() => {
          drawTemp();
          requestAnimationFrame(drawTemp);
        });
      }
    }

    function toggleTemperatureDetails() {
      setTemperatureDetails(homeTemperatureMode !== 'graph');
    }

    function setHomeTemperatureMode(mode) {
      setTemperatureDetails(mode === 'graph');
    }

    function setTemperatureWindow(seconds) {
      tempWindowSeconds = seconds === 180 ? 180 : seconds === 300 ? 300 : 600;
      if ($('tempWindow3Button')) $('tempWindow3Button').classList.toggle('active', tempWindowSeconds === 180);
      if ($('tempWindow5Button')) $('tempWindow5Button').classList.toggle('active', tempWindowSeconds === 300);
      if ($('tempWindow10Button')) $('tempWindow10Button').classList.toggle('active', tempWindowSeconds === 600);
      const context = tempWindowSeconds === 180 ? 'Последние 3 минуты' : tempWindowSeconds === 300 ? 'Последние 5 минут' : 'Последние 10 минут';
      if ($('tempChartContext')) $('tempChartContext').textContent = tr(context);
      drawTemp();
    }

    function temperatureWindowSeries() {
      const now = performance.now() / 1000;
      const start = now - tempWindowSeconds;
      return tempSeries.filter(point => Number(point.x) >= start).map(point => ({ ...point, x: Math.max(0, Number(point.x) - start) }));
    }

    function drawTemperatureTimeAxis(frame) {
      const ctx = frame.ctx;
      const step = tempWindowSeconds <= 300 ? 60 : 120;
      const ticks = [];
      for (let elapsed = 0; elapsed <= tempWindowSeconds; elapsed += step) ticks.push(elapsed);
      if (ticks[ticks.length - 1] !== tempWindowSeconds) ticks.push(tempWindowSeconds);
      ctx.save();
      ticks.forEach((elapsed, index) => {
        const ratio = elapsed / tempWindowSeconds;
        const x = frame.left + ratio * frame.plotW;
        ctx.strokeStyle = 'rgba(42,48,58,.72)';
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(x, frame.top);
        ctx.lineTo(x, frame.bottom);
        ctx.stroke();
        const remainingMinutes = Math.round((tempWindowSeconds - elapsed) / 60);
        const label = remainingMinutes === 0 ? '0' : '−' + remainingMinutes;
        ctx.font = chartFont(11);
        ctx.fillStyle = '#7f8793';
        ctx.textBaseline = 'top';
        ctx.textAlign = index === 0 ? 'left' : index === ticks.length - 1 ? 'right' : 'center';
        ctx.fillText(label, x, frame.bottom + 5);
      });
      ctx.fillStyle = '#7f8793';
      ctx.textAlign = 'right';
      ctx.fillText(tr('мин'), frame.w - 3, frame.bottom + 5);
      ctx.restore();
    }

    function temperatureChartScale() {
      const target = Number(activeTemperatureTarget());
      const steamScale = steamModeOn || (Number.isFinite(target) && target > 110);
      const steamMax = Number.isFinite(target)
        ? Math.ceil((target + 5) / 10) * 10
        : 150;
      return steamScale
        ? { min:90, max:Math.max(150, steamMax), step:10 }
        : { min:80, max:105, step:5 };
    }

    function drawTemperatureAxes(frame, series, scale = temperatureChartScale()) {
      const ctx = frame.ctx;
      ctx.font = chartFont(11);
      ctx.textBaseline = 'middle';
      for (let temp = scale.min; temp <= scale.max; temp += scale.step) {
        const y = yAt(frame, temp, scale.min, scale.max);
        ctx.strokeStyle = '#2a303a';
        ctx.beginPath();
        ctx.moveTo(frame.left, y);
        ctx.lineTo(frame.right, y);
        ctx.stroke();
        ctx.fillStyle = '#d5dae1';
        ctx.textAlign = 'right';
        ctx.fillText(temp + '°', frame.left - 6, y);
      }
      for (let power = 0; power <= 100; power += 20) {
        const y = yAt(frame, power, 0, 100);
        ctx.fillStyle = '#ff8b8b';
        ctx.textAlign = 'left';
        ctx.fillText(power + '%', frame.right + 7, y);
      }
      ctx.textBaseline = 'alphabetic';
      ctx.fillStyle = '#d5dae1';
      ctx.textAlign = 'left';
      ctx.fillText('°C', 2, 10);
      ctx.fillStyle = '#ff8b8b';
      ctx.textAlign = 'right';
      ctx.fillText(tr('нагрев'), frame.w - 3, 10);
      drawTemperatureTimeAxis(frame);
    }



    function drawHomePressureGauge() {
      const canvas = $('homePressureGauge');
      const card = $('homePressureCard');

      if (!canvas || !canvas.isConnected) return;

      const rect = canvas.getBoundingClientRect();
      if (rect.width < 72 || rect.height < 72) return;

      const dpr = Math.max(
        1,
        Math.min(2, Number(window.devicePixelRatio) || 1)
      );
      const pixelWidth = Math.round(rect.width * dpr);
      const pixelHeight = Math.round(rect.height * dpr);

      if (
        canvas.width !== pixelWidth ||
        canvas.height !== pixelHeight
      ) {
        canvas.width = pixelWidth;
        canvas.height = pixelHeight;
      }

      const ctx = canvas.getContext('2d');
      if (!ctx) return;

      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      ctx.clearRect(0, 0, rect.width, rect.height);

      const rawPressure = Number(values.pressure);
      const pressure = Number.isFinite(rawPressure)
        ? Math.max(0, Math.min(12, rawPressure))
        : 0;

      card?.classList.toggle('live', pressure > .15);

      const centerX = rect.width / 2;
      const centerY = rect.height / 2;
      const provisionalRadius = Math.max(
        36,
        Math.min(rect.width, rect.height) / 2 - 3
      );
      const ringWidth = Math.max(
        10,
        Math.min(14, provisionalRadius * .17)
      );
      const radius = Math.max(
        30,
        provisionalRadius - ringWidth / 2
      );
      const startAngle = Math.PI * .73;
      const sweep = Math.PI * 1.54;
      const endAngle = startAngle + sweep;
      const activeAngle = startAngle + (pressure / 12) * sweep;

      ctx.lineCap = 'round';
      ctx.strokeStyle = 'rgba(59,76,90,.54)';
      ctx.lineWidth = ringWidth;
      ctx.beginPath();
      ctx.arc(centerX, centerY, radius, startAngle, endAngle);
      ctx.stroke();

      ctx.lineCap = 'butt';
      ctx.strokeStyle = 'rgba(91,122,145,.18)';
      ctx.lineWidth = 1;
      ctx.beginPath();
      ctx.arc(
        centerX,
        centerY,
        radius - ringWidth * .95,
        0,
        Math.PI * 2
      );
      ctx.stroke();

      if (pressure > 0) {
        const activeGradient = ctx.createLinearGradient(
          centerX - radius,
          centerY,
          centerX + radius,
          centerY
        );
        activeGradient.addColorStop(0, '#55b8ee');
        activeGradient.addColorStop(.58, '#62c0f0');
        activeGradient.addColorStop(1, '#74caf4');

        ctx.lineCap = 'round';
        ctx.strokeStyle = activeGradient;
        ctx.lineWidth = ringWidth;
        ctx.beginPath();
        ctx.arc(centerX, centerY, radius, startAngle, activeAngle);
        ctx.stroke();
      }
    }

    function drawHomeTemperatureMini() {
      const canvas = $('homeTempMiniChart');
      if (!canvas || !canvas.isConnected) return;

      const rect = canvas.getBoundingClientRect();
      if (rect.width < 24 || rect.height < 18) return;

      const dpr = Math.max(1, Math.min(2, Number(window.devicePixelRatio) || 1));
      const pixelWidth = Math.round(rect.width * dpr);
      const pixelHeight = Math.round(rect.height * dpr);

      if (canvas.width !== pixelWidth || canvas.height !== pixelHeight) {
        canvas.width = pixelWidth;
        canvas.height = pixelHeight;
      }

      const ctx = canvas.getContext('2d');
      if (!ctx) return;

      ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
      ctx.clearRect(0, 0, rect.width, rect.height);

      const now = performance.now() / 1000;
      let series = tempSeries
        .filter(point =>
          Number.isFinite(Number(point?.v)) &&
          Number(point?.x) >= now - 180
        )
        .slice(-220);

      if (series.length < 2) {
        const current = Number(values.temp);
        const target = Number(activeTemperatureTarget());
        if (!Number.isFinite(current)) return;
        series = [
          {x:0, v:current, t:Number.isFinite(target) ? target : current},
          {x:1, v:current, t:Number.isFinite(target) ? target : current}
        ];
      }

      const temperatures = series.map(point => Number(point.v));
      const currentTarget = Number(activeTemperatureTarget());
      const historicalTargets = series
        .map(point => Number(point.t))
        .filter(Number.isFinite);
      const targets = Number.isFinite(currentTarget)
        ? [currentTarget]
        : historicalTargets;
      let minimum = Math.min(...temperatures, ...(targets.length ? targets : temperatures));
      let maximum = Math.max(...temperatures, ...(targets.length ? targets : temperatures));

      if (maximum - minimum < 1.4) {
        const center = (minimum + maximum) / 2;
        minimum = center - .7;
        maximum = center + .7;
      } else {
        minimum -= .35;
        maximum += .35;
      }

      const left = 3;
      const right = rect.width - 3;
      const top = 3;
      const bottom = rect.height - 3;
      const firstX = Number(series[0].x);
      const lastX = Number(series[series.length - 1].x);
      const spanX = Math.max(.001, lastX - firstX);
      const spanY = Math.max(.001, maximum - minimum);

      const xAt = point => left + ((Number(point.x) - firstX) / spanX) * (right - left);
      const yAt = value => bottom - ((Number(value) - minimum) / spanY) * (bottom - top);

      const targetValue = Number.isFinite(currentTarget)
        ? currentTarget
        : [...series].reverse()
          .map(point => Number(point.t))
          .find(Number.isFinite);

      if (Number.isFinite(targetValue)) {
        ctx.save();
        ctx.setLineDash([4, 4]);
        ctx.strokeStyle = 'rgba(75,209,139,.58)';
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(left, yAt(targetValue));
        ctx.lineTo(right, yAt(targetValue));
        ctx.stroke();
        ctx.restore();
      }

      ctx.beginPath();
      series.forEach((point, index) => {
        const x = xAt(point);
        const y = yAt(point.v);
        index ? ctx.lineTo(x, y) : ctx.moveTo(x, y);
      });
      ctx.lineTo(right, bottom);
      ctx.lineTo(left, bottom);
      ctx.closePath();

      const fill = ctx.createLinearGradient(0, top, 0, bottom);
      fill.addColorStop(0, 'rgba(225,139,72,.24)');
      fill.addColorStop(1, 'rgba(225,139,72,0)');
      ctx.fillStyle = fill;
      ctx.fill();

      ctx.beginPath();
      series.forEach((point, index) => {
        const x = xAt(point);
        const y = yAt(point.v);
        index ? ctx.lineTo(x, y) : ctx.moveTo(x, y);
      });
      ctx.strokeStyle = '#e7a06a';
      ctx.lineWidth = 2;
      ctx.lineJoin = 'round';
      ctx.lineCap = 'round';
      ctx.stroke();

      const last = series[series.length - 1];
      ctx.fillStyle = '#f2bb8e';
      ctx.beginPath();
      ctx.arc(xAt(last), yAt(last.v), 2.5, 0, Math.PI * 2);
      ctx.fill();
    }

    function drawTemp() {
      drawHomePressureGauge();
      drawHomeTemperatureMini();
      activeChartFontZone = 'temperature';

      const canvas = $('tempChart');
      if (!canvas || !canvas.isConnected || canvas.getBoundingClientRect().width < 20) return;
      const series = temperatureWindowSeries();
      const frame = canvasFrame('tempChart', { left: 39, right: 48, top: 18, bottom: 24 });
      const scale = temperatureChartScale();
      const activeTarget = Number(activeTemperatureTarget());
      const targetSeries = Number.isFinite(activeTarget)
        ? series.map(point => ({ ...point, t:activeTarget }))
        : series;
      drawTemperatureAxes(frame, series, scale);
      plotLine(frame, series, 'v', '#e18b48', scale.min, scale.max, 2.1);
      plotLine(frame, targetSeries, 't', '#4bd18b', scale.min, scale.max, 1.7);
      plotLine(frame, series, 'h', 'rgba(255,104,104,.6)', 0, 100, 1.3);
    }

    function path(domain, name, action, query) {
      let result = '/' + domain + '/' + encodeURIComponent(name);
      if (action) result += '/' + action;
      if (query) result += '?' + new URLSearchParams(query);
      return result;
    }

    let connectionLastOkAt = 0;
    let connectionOfflineTimer = 0;
    const connectionOfflineDelayMs = 10000;

    let eventStreamConnected = false;
    let eventStreamLastActivityAt = 0;
    let fallbackSyncBusy = false;
    let profileSnapshotSerial = 0;
    let profileStateCommitTimer = 0;
    const profileStateCommitDelayMs = 45;
    const eventStreamStaleMs = 30000;
    const fallbackSyncIntervalMs = 10000;

    function markEventStreamActivity() {
      eventStreamConnected = true;
      eventStreamLastActivityAt = Date.now();
      markDashboardOnline();
    }

    function markEventStreamError() {
      eventStreamConnected = false;
      scheduleDashboardOffline();
    }

    function eventStreamHealthy() {
      return (
        eventStreamConnected &&
        Date.now() - eventStreamLastActivityAt < eventStreamStaleMs
      );
    }

    function markDashboardOnline() {
      connectionLastOkAt = Date.now();
      if (connectionOfflineTimer) {
        clearTimeout(connectionOfflineTimer);
        connectionOfflineTimer = 0;
      }
      const node = $('online');
      if (!node) return;
      node.textContent = tr('онлайн');
      node.classList.add('ok');
    }

    function scheduleDashboardOffline() {
      if (connectionOfflineTimer) return;
      connectionOfflineTimer = setTimeout(() => {
        connectionOfflineTimer = 0;
        const silentFor = Date.now() - connectionLastOkAt;
        if (silentFor < connectionOfflineDelayMs) {
          scheduleDashboardOffline();
          return;
        }
        const node = $('online');
        if (!node) return;
        node.textContent = tr('нет связи');
        node.classList.remove('ok');
      }, connectionOfflineDelayMs);
    }

    async function post(domain, name, action, query) {
      const response = await fetch(path(domain, name, action, query), { method: 'POST' });
      if (!response.ok) throw Error(response.status);
      markDashboardOnline();
    }

    async function get(domain, name, detail = false) {
      const response = await fetch(
        path(domain, name) + (detail ? '?detail=all' : ''),
        { cache: 'no-store' }
      );
      if (!response.ok) throw Error(response.status);
      const data = await response.json();
      markDashboardOnline();
      return data;
    }

    async function shot(on) {
      try {
        let draftApplied = false;
        if (on && profileLocalDraftActive()) {
          const applied = await saveSettings({ silent:true });
          if (!applied) return;
          clearMainCurveDraftState();
          draftApplied = true;
        }
        if (on) await applyAdvancedPhaseSpec('');
        await post('switch', 'Silvia Brew Shot', on ? 'turn_on' : 'turn_off');
        toast(on
          ? (draftApplied
              ? tr('Ручные настройки сохранены как Custom') + '. ' + tr('Пролив запущен')
              : tr('Пролив запущен'))
          : tr('Пролив остановлен'));
      } catch (_) {
        toast(tr('Команда не выполнена'), true);
      }
    }

    async function machinePower() {
      if (powerCommandPending) return;

      const previous = machineOn;
      const desired = !previous;
      powerCommandPending = true;
      powerDesiredState = desired;
      powerCommandStartedAt = Date.now();

      // Immediate visual response; SSE remains the source of truth.
      showPower(desired, 'optimistic');

      try {
        await post('switch', 'Silvia Power Relay', desired ? 'turn_on' : 'turn_off');
        toast(tr(desired ? 'Кофемашина включена' : 'Кофемашина выключена'));

        // Normally SSE confirms almost immediately. If it does not, perform one
        // lightweight read so the optimistic state cannot remain stale.
        if (powerCommandPending) {
          if (powerConfirmTimer) clearTimeout(powerConfirmTimer);
          powerConfirmTimer = setTimeout(async () => {
            powerConfirmTimer = 0;
            if (!powerCommandPending) return;
            try {
              const state = await get('switch', 'Silvia Power Relay');
              powerCommandPending = false;
              powerDesiredState = null;
              powerCommandStartedAt = 0;
              showPower(stateOn(state.value ?? state.state), 'confirm');
            } catch (_) {
              // Keep waiting for SSE; the 4 s guard in showPower releases stale protection.
            }
          }, 1200);
        }
      } catch (_) {
        if (powerConfirmTimer) {
          clearTimeout(powerConfirmTimer);
          powerConfirmTimer = 0;
        }
        powerCommandPending = false;
        powerDesiredState = null;
        powerCommandStartedAt = 0;
        showPower(previous, 'rollback');
        toast(tr('Питание не изменено'), true);
      }
    }

    async function toggleShotAction(){ await shot(!shotRunning); }
    async function toggleHotWater(){try{const next=!hotWaterOn;await post('switch','Silvia Hot Water',next?'turn_on':'turn_off');setHotWater(next);toast(tr(next?'Вода включена':'Вода выключена'));}catch(_){toast(tr('Команда не выполнена'),true);}}
    async function toggleSteamMode(){try{const next=!steamModeOn;await post('switch','Silvia Steam Mode',next?'turn_on':'turn_off');setSteamMode(next);toast(tr(next?'Пар включён':'Пар выключен'));}catch(_){toast(tr('Команда не выполнена'),true);}}

    function profileOptionsFromData(data) {
      const candidates = [
        data?.options,
        data?.option,
        data?.traits?.options
      ];

      for (const candidate of candidates) {
        if (!Array.isArray(candidate)) continue;
        const list = candidate
          .map(option => String(option ?? '').trim())
          .filter(Boolean);
        if (list.length) return list;
      }

      return [];
    }

    function readProfileCfgFromInputs() {
      const raw = {
        prePump:inputNumber('prePump'),
        prePause:inputNumber('prePause'),
        prePower:inputNumber('prePower'),
        mainPressure:inputNumber('mainPressure'),
        endPressure:inputNumber('endPressure'),
        shotSeconds:inputNumber('shotSeconds'),
        softInfusionTime:inputNumber('softInfusionTime'),
        softInfusionStartPower:inputNumber('softInfusionStartPower')
      };

      profileCfg = raw;
      updatePressureTarget();
      updateQuickActions();
      updateShotScenario();
    }

    const ALL_NUMBER_GROUPS = [
      nums,
      machineNums,
      temperatureNums,
      pressureNums,
      scaleNums,
      backflushNums
    ];

    /*
     * Native ESPHome-style state path:
     * /events is the primary source of truth for both the initial entity dump
     * and subsequent updates. REST GET is kept only as a fallback when SSE is
     * unavailable. Number entities are therefore indexed once and updated
     * directly from their SSE state events.
     */
    const NUMBER_ENTITY_LOOKUP = new Map();
    ALL_NUMBER_GROUPS.forEach(group => {
      Object.entries(group).forEach(([id, name]) => {
        NUMBER_ENTITY_LOOKUP.set(clean(name), { id, name });
      });
    });
    const PROFILE_NUMBER_ENTITY_KEYS = new Set(
      Object.values(nums).map(name => clean(name))
    );

    function profileLocalDraftActive() {
      return !!(
        recipeDirty ||
        mainCurveDirty ||
        mainCurveEditing ||
        mainCurveDragKey
      );
    }

    function commitProfileStateFromSse() {
      if (profileStateCommitTimer) {
        clearTimeout(profileStateCommitTimer);
        profileStateCommitTimer = 0;
      }

      // Do not let delayed ESP state overwrite a curve that is being edited
      // locally. Profile application/save paths clear or explicitly allow the
      // draft before their state burst arrives.
      if (
        profileLocalDraftActive() &&
        !homeProfileApplying &&
        !recipeApplying &&
        !userProfileApplying &&
        !homeQuickSaving
      ) {
        return;
      }

      readProfileCfgFromInputs();

      if (!profileLocalDraftActive()) {
        const profile = currentProfileName || 'Custom';
        recipeBaseProfile = profile;
        captureRecipeBaseline(profile);
      }

      chartMode = 'auto';
      updateRecipeProfileUi();
      updateRecipeDescription();
      updateShotScenario();
      updateChartControls();
      updateMainCurveEditor();

      requestAnimationFrame(() => {
        draw();
        drawRecipePreview();
      });
    }

    function scheduleProfileStateCommitFromSse(delay = profileStateCommitDelayMs) {
      if (profileStateCommitTimer) clearTimeout(profileStateCommitTimer);
      profileStateCommitTimer = setTimeout(() => {
        profileStateCommitTimer = 0;
        commitProfileStateFromSse();
      }, delay);
    }

    let exactPidStateSeen = false;

    function applyExactPidStateFromSse(key, event) {
      if (!key.endsWith('silvia pid parameters exact')) return false;

      const parts = String(event.value ?? event.state ?? '')
        .split('|')
        .map(Number);

      if (parts.length === 3 && parts.every(Number.isFinite)) {
        exactPidStateSeen = true;
        setNumberEntityValue('Silvia PID KP', parts[0]);
        setNumberEntityValue('Silvia PID KI', parts[1]);
        setNumberEntityValue('Silvia PID KD', parts[2]);
      }

      return true;
    }

    function applyNumberStateFromSse(key, event) {
      let lookupKey = key;
      let entry = NUMBER_ENTITY_LOOKUP.get(lookupKey);
      if (!entry) {
        const match = [...NUMBER_ENTITY_LOOKUP.entries()]
          .find(([entityKey]) => key.endsWith(entityKey));
        if (match) {
          lookupKey = match[0];
          entry = match[1];
        }
      }
      if (!entry) return false;

      const isProfileNumber = PROFILE_NUMBER_ENTITY_KEYS.has(lookupKey);
      const localDraftProtected = !!(
        isProfileNumber &&
        profileLocalDraftActive() &&
        !homeProfileApplying &&
        !recipeApplying &&
        !userProfileApplying &&
        !homeQuickSaving
      );

      if (localDraftProtected) return true;

      const value = event.value ?? event.state;
      const roundedPidNumber =
        exactPidStateSeen &&
        ['pidKp', 'pidKi', 'pidKd'].includes(entry.id);

      if (
        value !== undefined &&
        value !== null &&
        !roundedPidNumber
      ) {
        setNumberEntityValue(entry.name, value);
      }

      // DETAIL_ALL on the initial EventSource connection carries Number
      // traits. Mirror them into our controls just like the stock ESPHome UI.
      const input = $(entry.id);
      if (input) {
        if (event.min_value !== undefined) input.min = String(event.min_value);
        if (event.max_value !== undefined) input.max = String(event.max_value);
        if (event.step !== undefined) input.step = String(event.step);
      }

      if (isProfileNumber) {
        // A preset publishes several Number states in a short burst. Coalesce
        // them so our graph is redrawn once with a coherent profile snapshot,
        // without issuing any REST reads.
        scheduleProfileStateCommitFromSse();
      }

      return true;
    }

    function setNumberEntityValue(entityName, value) {
      const numeric = Number(value);

      ALL_NUMBER_GROUPS.forEach(group => {
        Object.entries(group).forEach(([id, name]) => {
          if (name !== entityName) return;
          const input = $(id);
          if (!input) return;

          if (
            ['pidKp', 'pidKi', 'pidKd'].includes(id) &&
            Number.isFinite(numeric)
          ) {
            input.value = String(numeric);
          } else {
            input.value = value;
          }
        });
      });
    }

    async function loadNumberControls(groups) {
      const entries = Object.entries(Object.assign({}, ...groups));
      const results = await Promise.all(entries.map(async ([id, name]) => {
        try { return [id, name, await get('number', name)]; } catch (_) { return [id, name, null]; }
      }));

      results.forEach(([, name, data]) => {
        if (!data) return;
        const value = data.value ?? data.state;
        if (value !== undefined && value !== null) {
          setNumberEntityValue(name, value);
        }
      });
    }

    async function loadSwitchControls(ids = Object.keys(switchControls)) {
      const entries = ids
        .map(id => [id, switchControls[id]])
        .filter(([, name]) => !!name);

      await Promise.all(entries.map(async ([id, name]) => {
        try {
          const data = await get('switch', name);
          const node = $(id);
          if (node) node.checked = stateOn(data.value ?? data.state);
        } catch (_) {}
      }));

      updateStopByWeightControl();
      syncHomeProfileEditor();
    }

    async function loadExactPidControls() {
      try {
        const data = await get(
          'text_sensor',
          'Silvia PID Parameters Exact'
        );
        const parts = String(data.value ?? data.state ?? '')
          .split('|')
          .map(Number);

        if (parts.length !== 3 || !parts.every(Number.isFinite)) return;

        setNumberEntityValue('Silvia PID KP', parts[0]);
        setNumberEntityValue('Silvia PID KI', parts[1]);
        setNumberEntityValue('Silvia PID KD', parts[2]);
      } catch (_) {}
    }


    async function loadHomeProfileSnapshot(showPreview = false) {
      /*
       * Home only needs the active profile plus the small group of numbers
       * used by the shot card. Loading the entire settings page here caused
       * dozens of unnecessary REST requests and could race with SSE profile
       * updates.
       */
      const serial = ++profileSnapshotSerial;

      try {
        const numberEntries = Object.entries(nums);
        const results = await Promise.all([
          get('select', 'Silvia Shot Profile', true),
          ...numberEntries.map(([, name]) => get('number', name))
        ]);

        if (serial !== profileSnapshotSerial) return false;

        const profileData = results[0];
        const fetchedOptions = profileOptionsFromData(profileData);
        if (fetchedOptions.length) {
          availableProfileOptions = fetchedOptions;
        }

        numberEntries.forEach(([, name], index) => {
          const data = results[index + 1];
          if (!data) return;
          const value = data.value ?? data.state;
          if (value !== undefined && value !== null) {
            setNumberEntityValue(name, value);
          }
        });

        const nextProfile =
          String(profileData.value ?? profileData.state ?? '') ||
          currentProfileName ||
          'Custom';

        if (!availableProfileOptions.length && nextProfile) {
          availableProfileOptions = [nextProfile];
        }
        const options = availableProfileOptions;
        currentProfileName = nextProfile;

        const select = $('profileSelect');
        if (select) {
          select.innerHTML = '';
          options.forEach(option =>
            select.add(
              new Option(
                profileLabel(option),
                option,
                false,
                option === nextProfile
              )
            )
          );
          select.value = nextProfile;
          select.onchange = applySelectedProfile;
        }

        fillHomeProfileSelect(options, nextProfile);
        syncProfileControls(nextProfile);

        if (
          !recipeDirty &&
          !mainCurveDirty &&
          !mainCurveEditing &&
          !mainCurveDragKey
        ) {
          readProfileCfgFromInputs();
          recipeBaseProfile = nextProfile;
          captureRecipeBaseline(nextProfile);
        }

        if (showPreview) chartMode = 'auto';

        updateRecipeProfileUi();
        updateRecipeDescription();
        updateShotScenario();
        updateChartControls();
        updateMainCurveEditor();

        requestAnimationFrame(() => {
          draw();
          drawRecipePreview();

          requestAnimationFrame(() => {
            draw();
          });
        });

        return true;
      } catch (_) {
        return false;
      }
    }


    const SETTINGS_STATUS_IDS = new Set([
      'pressureFeedForward',
      'pressureRiseRate',
      'tempFeedForward',
      'pidP',
      'pidI',
      'pidD',
      'piOutput',
      'warmupStatus',
      'autotuneStatus'
    ]);

    const MAINTENANCE_STATUS_IDS = new Set([
      'csvReady',
      'backflushStatus',
      'resetReason',
      'backflushShots',
      'lifetimeShots',
      'groundsUsed',
      'autoOffRemaining'
    ]);

    async function refreshStatus(ids = null) {
      const entities = ids
        ? statusEntities.filter(([, , id]) => ids.has(id))
        : statusEntities;

      return Promise.all(entities.map(async ([domain, name, id, type]) => {
        try {
          const data = await get(domain, name);
          const value = data.value ?? data.state;
          renderStatusValue(id, type, value);
          updateDiagnosticMirrors(id, value);
        } catch (_) {}
      })).then(() => {
        health();
        updateXdbHumanStatus();
      });
    }

    async function loadSettings() {
      // With a healthy EventSource the initial DETAIL_ALL dump has already
      // populated these controls, and future changes arrive as state events.
      // This mirrors the stock ESPHome dashboard and avoids duplicate GETs.
      if (eventStreamHealthy()) return;

      await Promise.all([
        loadNumberControls([
          machineNums,
          temperatureNums,
          pressureNums
        ])
      ]);

      // REST Number formatting can lose PID precision; this compact text
      // snapshot is retained only for the no-SSE fallback path.
      await loadExactPidControls();
      await refreshStatus(SETTINGS_STATUS_IDS);
    }

    async function loadMaintenanceSettings() {
      if (eventStreamHealthy()) return;

      await Promise.all([
        loadNumberControls([
          scaleNums,
          backflushNums
        ]),
        loadSwitchControls([
          'scaleAutoTare',
          'shotDiagnostics'
        ])
      ]);

      await refreshStatus(MAINTENANCE_STATUS_IDS);
    }


    async function applySelectedProfile() {
      const select = $('profileSelect');
      const option = select?.value;
      if (!option || recipeApplying) return;

      const previous = currentProfileName;
      if (option === 'Custom' && !customProfileAvailable) {
        toast(tr('Измените любой профиль и нажмите «Применить»'),true);
        select.value = previous;
        return;
      }
      recipeApplying = true;
      recipeDirty = false;
      clearMainCurveDraftState();

      // Match the stock ESPHome select behaviour: the control responds
      // immediately, while the ESP-published states arrive asynchronously.
      currentProfileName = option;
      recipeBaseProfile = option;
      advancedPhaseDraft = null;
      advancedPhaseSelected = -1;
      advancedPhaseEditorOpen = false;
      profilePhaseEditMode = false;
      activeCustomPhaseSpec = option === 'Custom' ? activeCustomPhaseSpec : '';
      activeCustomPhaseBase = option === 'Custom' ? activeCustomPhaseBase : '';
      renderAdvancedPhaseEditor();
      syncProfileControls(option);
      updateRecipeProfileUi();
      updateRecipeDescription();
      chartMode = 'auto';
      updateChartControls();
      updateMainCurveEditor();
      draw();
      drawRecipePreview();

      try {
        await post('select', 'Silvia Shot Profile', 'set', { option });
        toast(message('profileApplied', profileLabel(option)));
      } catch (_) {
        currentProfileName = previous;
        recipeBaseProfile = previous || recipeBaseProfile;
        syncProfileControls(previous);
        updateRecipeProfileUi();
        updateRecipeDescription();
        updateMainCurveEditor();
        draw();
        drawRecipePreview();
        toast(tr('Профиль не применён'), true);

        // REST is only a recovery path when the event stream itself is down.
        if (!eventStreamHealthy()) await loadHomeProfileSnapshot(false);
      } finally {
        recipeApplying = false;
      }
    }

    async function applyHomeProfile(select) {
      const option = select?.value;
      const hasDraft = !!(
        mainCurveDirty ||
        recipeDirty ||
        profilePhaseEditMode ||
        mainCurveEditing
      );

      if (!option || homeProfileApplying) return;
      if (option === 'Custom' && !customProfileAvailable) {
        toast(tr('Измените любой профиль и нажмите «Применить»'),true);
        return;
      }
      if (option === 'Custom' && hasDraft) return;
      if (option === currentProfileName && !hasDraft) return;

      activeUserProfileId = '';
      renderUserProfiles();

      const previous = currentProfileName;
      homeProfileApplying = true;
      recipeDirty = false;
      clearMainCurveDraftState();

      // Optimistic UI, exactly like a native ESPHome select. Profile Number
      // states then stream back over /events and redraw the graph in one
      // coalesced frame. No waitForProfile(), sleeps or REST rereads.
      currentProfileName = option;
      recipeBaseProfile = option;
      advancedPhaseDraft = null;
      advancedPhaseSelected = -1;
      advancedPhaseEditorOpen = false;
      profilePhaseEditMode = false;
      activeCustomPhaseSpec = option === 'Custom' ? activeCustomPhaseSpec : '';
      activeCustomPhaseBase = option === 'Custom' ? activeCustomPhaseBase : '';
      renderAdvancedPhaseEditor();
      syncProfileControls(option);
      chartMode = 'auto';
      updateRecipeProfileUi();
      updateRecipeDescription();
      updateMainCurveEditor();
      updateQuickActions();
      draw();
      drawRecipePreview();

      try {
        await post('select', 'Silvia Shot Profile', 'set', { option });
        toast(message('profileApplied', profileLabel(option)));
      } catch (_) {
        currentProfileName = previous;
        recipeBaseProfile = previous || recipeBaseProfile;
        syncProfileControls(previous);
        updateRecipeProfileUi();
        updateRecipeDescription();
        updateMainCurveEditor();
        draw();
        drawRecipePreview();
        toast(tr('Профиль не применён'), true);

        if (!eventStreamHealthy()) await loadHomeProfileSnapshot(false);
      } finally {
        homeProfileApplying = false;
        updateQuickActions();
      }
    }

    async function saveSettings(options = {}) {
      const silent = !!options.silent;
      let saved = false;
      const saveButton = $('recipeSaveButton');
      const resetButton = $('recipeResetButton');
      if (saveButton) saveButton.disabled = true;
      if (resetButton) resetButton.disabled = true;
      try {
        for (const [id, name] of Object.entries(nums)) {
          const node = $(id);
          if (node) {
            await post('number', name, 'set', { value: node.value });
            setNumberEntityValue(name, node.value);
          }
        }
        await applyAdvancedPhaseSpec(profilePhaseEditMode && advancedPhaseDraft ? advancedPhaseSpec(advancedPhaseDraft) : String(activeCustomPhaseSpec || ''));
        await post('select', 'Silvia Shot Profile', 'set', { option:'Custom' });
        recipeDirty = false;
        currentProfileName = 'Custom';
        recipeBaseProfile = 'Custom';
        syncProfileControls('Custom');
        readProfileCfgFromInputs();
        captureRecipeBaseline('Custom');
        updateMainCurveEditor();
        saved = true;
        if (!silent) toast(tr('Ручные настройки сохранены как Custom'));
      } catch (_) {
        toast(tr('Ошибка сохранения'), true);
        updateRecipeProfileUi();
      } finally {
        if (saveButton) saveButton.disabled = false;
        if (resetButton) resetButton.disabled = !recipeDirty;
      }
      return saved;
    }

    async function saveNumberGroup(group, successMessage = tr('Настройки сохранены')) {
      try {
        for (const [id, name] of Object.entries(group)) {
          const node = $(id);
          if (!node) continue;
          await post('number', name, 'set', { value: node.value });
          setNumberEntityValue(name, node.value);
        }
        toast(successMessage);
        if (!eventStreamHealthy()) await refreshStatus();
      } catch (_) {
        toast(tr('Ошибка сохранения'), true);
      }
    }

    function updateStopByWeightControl() {
      const input = $('stopByWeight');
      const chip = $('stopByWeightChip');
      const state = $('homeStopByWeightState');
      const enabled = !!input?.checked;

      if (chip) {
        chip.classList.toggle('active', enabled);
        chip.classList.toggle('disabled', !!input?.disabled);
        chip.title = tr(enabled ? 'Остановка по весу включена' : 'Остановка по весу выключена');
      }
      if (state) state.textContent = tr(enabled ? 'Вкл' : 'Выкл');
    }

    async function updateSwitch(input, name) {
      const desired = !!input.checked;
      input.disabled = true;
      updateStopByWeightControl();
      try {
        await post('switch', name, desired ? 'turn_on' : 'turn_off');
        toast(tr(desired ? 'Включено' : 'Выключено'));
      } catch (_) {
        input.checked = !desired;
        toast(tr('Переключатель не изменён'), true);
      } finally {
        input.disabled = false;
        updateStopByWeightControl();
      }
    }

    async function updateSelect(select, name) {
      select.disabled = true;
      try {
        await post('select', name, 'set', { option: select.value });
        toast(message('modeApplied', pidModeLabel(select.value)));
      } catch (_) {
        toast(tr('Режим не изменён'), true);
      } finally {
        select.disabled = false;
      }
    }

    async function pressButton(name, confirmation = '') {
      if (confirmation && !confirm(confirmation)) return false;
      try {
        await post('button', name, 'press');
        toast(tr('Команда отправлена'));
        if (!eventStreamHealthy()) setTimeout(refreshStatus, 450);
        return true;
      } catch (_) {
        toast(tr('Команда не выполнена'), true);
        return false;
      }
    }

    async function calibrateScale() {
      try {
        const node = $('scaleCalibrationWeight');
        await post('number', scaleNums.scaleCalibrationWeight, 'set', { value: node.value });
        await post('button', 'Silvia Scale Calibrate', 'press');
        toast(tr('Калибровка запущена'));
        if (!eventStreamHealthy()) setTimeout(refreshStatus, 450);
      } catch (_) {
        toast(tr('Калибровка не запущена'), true);
      }
    }

    const TRAINING_ISSUES = [
      [1 << 0, 'Нет pressure-фазы'],
      [1 << 1, 'Слишком короткая запись'],
      [1 << 2, 'Ошибки обмена XDB401'],
      [1 << 3, 'Некорректные отсчёты давления'],
      [1 << 4, 'Устаревшие данные давления'],
      [1 << 5, 'Сработал SENSOR_FAIL'],
      [1 << 6, 'Ошибки XDB401 подряд'],
      [1 << 7, 'Пропуски временной шкалы'],
      [1 << 8, 'Неправдоподобный скачок давления'],
      [1 << 9, 'Нет полных обучающих окон']
    ];

    function trainingRecords() {
      const records = Array.isArray(trainingDataset?.records)
        ? trainingDataset.records
        : [];
      return records
        .map(item => ({ ...(item?.record || {}), pinned:!!item?.pinned }))
        .filter(record => record.collected && Number(record.id) > 0)
        .sort((left, right) => Number(right.id) - Number(left.id));
    }

    function trainingStats() {
      const records = trainingRecords();
      const clean = records.filter(record => record.status === 'clean');
      const partial = records.filter(record => record.status === 'partial');
      const rejected = records.filter(record => record.status === 'rejected');
      const usableWindows = clean.reduce(
        (total, record) => total + (Number(record.usable_windows) || 0),
        0
      );
      const profiles = new Set(
        clean.map(record => String(record.profile || '').trim()).filter(Boolean)
      );
      const sensorIssueMask =
        (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) |
        (1 << 6) | (1 << 7) | (1 << 8);
      const sensorRejected = records.filter(
        record => (Number(record.issues) & sensorIssueMask) !== 0
      ).length;
      const score = Math.min(100, Math.round(
        Math.min(clean.length / 50, 1) * 65 +
        Math.min(usableWindows / 3000, 1) * 20 +
        Math.min(profiles.size / 3, 1) * 15
      ));
      let level = 'collecting';
      let title = 'Набор собирается';
      if (!records.length) {
        title = 'Данных пока нет';
      } else if (records.length >= 5 && rejected.length / records.length > .5) {
        level = 'problem';
        title = 'Проблемы с данными XDB401';
      } else if (clean.length >= 50 && usableWindows >= 3000 && profiles.size >= 3) {
        level = 'high';
        title = 'Высокая готовность';
      } else if (clean.length >= 20 && usableWindows >= 1000 && profiles.size >= 2) {
        level = 'prototype';
        title = 'Данных достаточно для пробной модели';
      }
      return {
        records, clean, partial, rejected, usableWindows,
        profiles:profiles.size, sensorRejected, score, level, title
      };
    }

    function trainingIssueText(record) {
      const issues = Number(record?.issues) || 0;
      const parts = TRAINING_ISSUES
        .filter(([mask]) => (issues & mask) !== 0)
        .map(([, label]) => tr(label));
      if (parts.length) return parts.join(' · ');
      if (record?.status === 'clean') return tr('Запись прошла строгую проверку XDB401');
      if (record?.status === 'partial') return tr('Подходят только отдельные непрерывные окна');
      return tr('Запись исключена из обучения');
    }

    function trainingStatusLabel(status) {
      if (status === 'clean') return tr('Чистый');
      if (status === 'partial') return tr('Частичный');
      return tr('Брак данных');
    }

    function trainingFormatBytes(bytes) {
      const value = Math.max(0, Number(bytes) || 0);
      if (value < 1024) return Math.round(value) + ' B';
      if (value < 1024 * 1024) return fmt(value / 1024, 1) + ' KiB';
      return fmt(value / (1024 * 1024), 2) + ' MiB';
    }

    function renderTrainingDataset() {
      const list = $('trainingRecordList');
      if (!list || !trainingDataset) return;
      const stats = trainingStats();
      const readiness = $('trainingReadiness');
      readiness.className = 'lab-readiness ' + stats.level;
      $('trainingReadinessTitle').textContent = tr(stats.title);
      $('trainingReadinessScore').textContent = stats.score + '%';
      $('trainingReadinessBar').style.width = stats.score + '%';
      $('trainingReadinessCopy').textContent = stats.records.length
        ? tr('Чистых проливов') + ': ' + stats.clean.length + ' ' + tr('из 50') +
          ' · ' + tr('Профилей в чистом наборе') + ': ' + stats.profiles
        : tr('Ни одного пролива для модели ещё не сохранено.');
      $('trainingCleanCount').textContent = stats.clean.length;
      $('trainingWindowCount').textContent = stats.usableWindows.toLocaleString(
        currentLanguage === 'en' ? 'en-US' : 'ru-RU'
      );
      $('trainingPartialCount').textContent = stats.partial.length;
      $('trainingRejectedCount').textContent = stats.rejected.length;
      $('trainingProfileCount').textContent = stats.profiles;
      $('trainingSensorRejectedCount').textContent = stats.sensorRejected;

      const collectionToggle = $('trainingCollectionToggle');
      collectionToggle.checked = !!trainingDataset.collection_enabled;
      const storage = trainingDataset.storage || {};
      const total = Number(storage.total_bytes) || 0;
      const used = Number(storage.used_bytes) || 0;
      const free = Math.max(0, Number(storage.free_bytes) || total - used);
      const usedPercent = total > 0 ? Math.min(100, used / total * 100) : 0;
      $('trainingStorageText').textContent =
        trainingFormatBytes(used) + ' / ' + trainingFormatBytes(total) +
        ' · ' + trainingFormatBytes(free) + ' ' + tr('свободно');
      $('trainingStorageBar').style.width = usedPercent + '%';
      const averageBytes = stats.records.length
        ? stats.records.reduce(
            (sum, record) => sum + (Number(record.csv_bytes) || 0) + 3072,
            0
          ) / stats.records.length
        : 0;
      $('trainingStorageEstimate').textContent = averageBytes > 0
        ? tr('Примерно поместится ещё') + ': ' +
          Math.max(0, Math.floor(Math.max(0, free - 65536) / averageBytes)) +
          ' ' + tr('полных проливов')
        : '';

      if (!stats.records.length) {
        list.innerHTML = '<div class="lab-empty">' +
          archiveEscapeHtml(tr('Ни одного пролива для модели ещё не сохранено.')) +
          '</div>';
        return;
      }
      list.innerHTML = stats.records.map(record => {
        const id = Number(record.id);
        const status = ['clean','partial','rejected'].includes(record.status)
          ? record.status
          : 'rejected';
        const time = archiveTimeLabel(record, record, id);
        const windows = Number(record.usable_windows) || 0;
        return '<div class="lab-record ' + status + '">' +
          '<i class="lab-record-dot"></i>' +
          '<div class="lab-record-copy">' +
            '<div class="lab-record-title"><b>#' + id + ' · ' +
              archiveEscapeHtml(profileLabel(record.profile || '')) + '</b>' +
              '<span>' + archiveEscapeHtml(trainingStatusLabel(status)) + '</span></div>' +
            '<small>' + archiveEscapeHtml(time) + ' · ' + windows + ' ' + tr('окон') +
              ' · ' + archiveEscapeHtml(trainingIssueText(record)) + '</small>' +
          '</div>' +
          '<button type="button" class="lab-pin' + (record.pinned ? ' active' : '') +
            '" onclick="toggleTrainingPin(' + id + ',' + (!record.pinned) + ')"' +
            ' aria-label="' + archiveEscapeHtml(tr(record.pinned ? 'Открепить' : 'Закрепить')) +
            '" title="' + archiveEscapeHtml(tr(record.pinned ? 'Открепить' : 'Закрепить')) +
            '">' + (record.pinned ? '★' : '☆') + '</button>' +
        '</div>';
      }).join('');
    }

    async function loadTrainingDataset(force = false) {
      if (trainingDatasetBusy || (trainingDataset && !force)) return;
      trainingDatasetBusy = true;
      try {
        const response = await fetch('/training/index.json', { cache:'no-store' });
        if (!response.ok) throw Error('training index unavailable');
        trainingDataset = await response.json();
        renderTrainingDataset();
      } catch (_) {
        const list = $('trainingRecordList');
        if (list) list.innerHTML = '<div class="lab-empty">' +
          archiveEscapeHtml(tr('Архив пока недоступен')) + '</div>';
      } finally {
        trainingDatasetBusy = false;
      }
    }

    async function toggleTrainingCollection(input) {
      const desired = !!input.checked;
      input.disabled = true;
      try {
        const response = await fetch(
          '/training/collection/' + (desired ? 'on' : 'off'),
          { method:'POST', cache:'no-store' }
        );
        if (!response.ok) throw Error('collection update failed');
        if (trainingDataset) trainingDataset.collection_enabled = desired;
        toast(tr(desired ? 'Сбор включён' : 'Сбор выключен'));
      } catch (_) {
        input.checked = !desired;
        toast(tr('Не удалось изменить режим сбора'), true);
      } finally {
        input.disabled = false;
      }
    }

    async function toggleTrainingPin(id, pinned) {
      try {
        const response = await fetch(
          '/training/' + (pinned ? 'pin/' : 'unpin/') + Number(id),
          { method:'POST', cache:'no-store' }
        );
        if (!response.ok) throw Error('pin update failed');
        await loadTrainingDataset(true);
      } catch (_) {
        toast(tr('Не удалось изменить закрепление'), true);
      }
    }

    function trainingTarWrite(target, offset, text, length) {
      const encoded = new TextEncoder().encode(String(text));
      target.set(encoded.subarray(0, length), offset);
    }

    function trainingTarOctal(value, length) {
      return Math.max(0, Number(value) || 0)
        .toString(8)
        .padStart(length - 1, '0')
        .slice(-(length - 1)) + '\0';
    }

    function trainingTarHeader(name, size, modifiedSeconds) {
      const header = new Uint8Array(512);
      trainingTarWrite(header, 0, name, 100);
      trainingTarWrite(header, 100, '0000644\0', 8);
      trainingTarWrite(header, 108, '0000000\0', 8);
      trainingTarWrite(header, 116, '0000000\0', 8);
      trainingTarWrite(header, 124, trainingTarOctal(size, 12), 12);
      trainingTarWrite(header, 136, trainingTarOctal(modifiedSeconds, 12), 12);
      header.fill(32, 148, 156);
      header[156] = '0'.charCodeAt(0);
      trainingTarWrite(header, 257, 'ustar\0', 6);
      trainingTarWrite(header, 263, '00', 2);
      let checksum = 0;
      header.forEach(byte => { checksum += byte; });
      trainingTarWrite(
        header,
        148,
        checksum.toString(8).padStart(6, '0').slice(-6) + '\0 ',
        8
      );
      return header;
    }

    function trainingTarAdd(parts, name, blob, modifiedSeconds) {
      parts.push(trainingTarHeader(name, blob.size, modifiedSeconds));
      parts.push(blob);
      const padding = (512 - blob.size % 512) % 512;
      if (padding) parts.push(new Uint8Array(padding));
    }

    async function exportTrainingDataset(includeAll) {
      if (trainingExportBusy) return;
      if (!trainingDataset) await loadTrainingDataset(true);
      const stats = trainingStats();
      const records = includeAll
        ? stats.records
        : stats.clean.filter(record => record.eligible !== false);
      if (!records.length) {
        toast(tr('Нет подходящих записей для выгрузки'), true);
        return;
      }
      trainingExportBusy = true;
      const progress = $('trainingExportProgress');
      const buttons = [$('trainingExportButton'), $('trainingExportAllButton')];
      buttons.forEach(button => { if (button) button.disabled = true; });
      progress.textContent = tr('Готовим архив…');
      try {
        const parts = [];
        const exported = [];
        const modified = Math.floor(Date.now() / 1000);
        for (let index = 0; index < records.length; index++) {
          const record = records[index];
          const stem = archiveStem(Number(record.id));
          progress.textContent = tr('Скачиваем') + ' ' + (index + 1) + ' / ' + records.length;
          for (const extension of ['csv','json']) {
            const response = await fetch(
              '/shots/file/' + stem + '.' + extension,
              { cache:'no-store' }
            );
            if (!response.ok) throw Error('training file unavailable');
            trainingTarAdd(
              parts,
              'shots/' + stem + '.' + extension,
              await response.blob(),
              modified
            );
          }
          exported.push(record);
          await sleep(15);
        }
        const manifest = {
          format:'rancilio-pressure-training-v1',
          created_at:new Date().toISOString(),
          filter:includeAll ? 'all_collected' : 'strict_clean',
          readiness:{
            score:stats.score,
            level:stats.level,
            clean_shots:stats.clean.length,
            usable_windows:stats.usableWindows,
            profiles:stats.profiles
          },
          records:exported
        };
        trainingTarAdd(
          parts,
          'manifest.json',
          new Blob([JSON.stringify(manifest, null, 2) + '\n'], {type:'application/json'}),
          modified
        );
        parts.push(new Uint8Array(1024));
        const archive = new Blob(parts, {type:'application/x-tar'});
        const link = document.createElement('a');
        const now = new Date();
        const stamp = now.getFullYear() +
          String(now.getMonth() + 1).padStart(2, '0') +
          String(now.getDate()).padStart(2, '0') + '-' +
          String(now.getHours()).padStart(2, '0') +
          String(now.getMinutes()).padStart(2, '0');
        link.href = URL.createObjectURL(archive);
        link.download = 'rancilio-training-' + stamp + '.tar';
        link.click();
        setTimeout(() => URL.revokeObjectURL(link.href), 10000);
        progress.textContent = tr('Архив готов') + ': ' + records.length;
      } catch (_) {
        progress.textContent = '';
        toast(tr('Не удалось выгрузить набор'), true);
      } finally {
        trainingExportBusy = false;
        buttons.forEach(button => { if (button) button.disabled = false; });
      }
    }

    function archiveEscapeHtml(value) {
      return String(value ?? '')
        .replaceAll('&', '&amp;')
        .replaceAll('<', '&lt;')
        .replaceAll('>', '&gt;')
        .replaceAll('"', '&quot;')
        .replaceAll("'", '&#039;');
    }

    function archiveShotId(value) {
      if (value && typeof value === 'object') {
        return Number(
          value.id ??
          value.shot_id ??
          value.number
        ) || 0;
      }

      return Number(value) || 0;
    }

    function archiveStem(id) {
      return (
        'shot-' +
        String(id).padStart(6, '0')
      );
    }

    function archiveSummaryValue(
      summary,
      paths,
      fallback = NaN
    ) {
      for (const path of paths) {
        let value = summary;

        for (const key of path.split('.')) {
          value = value?.[key];
        }

        const number = Number(value);
        if (Number.isFinite(number)) {
          return number;
        }
      }

      return fallback;
    }

    function archiveDate(summary, raw) {
      const candidates = [
        summary?.timestamp,
        summary?.created_at,
        summary?.datetime,
        summary?.date,
        summary?.started_at,
        summary?.finished_at,
        raw?.timestamp,
        raw?.created_at,
        raw?.datetime,
        raw?.date
      ];

      for (const value of candidates) {
        if (value === null || value === undefined || value === '') {
          continue;
        }

        let date;

        if (
          typeof value === 'number' ||
          /^\d{10,13}$/.test(String(value))
        ) {
          const number = Number(value);
          if (!Number.isFinite(number) || number < 1577836800) {
            continue;
          }
          date = new Date(
            number < 1e12
              ? number * 1000
              : number
          );
        } else {
          date = new Date(value);
        }

        if (!Number.isNaN(date.getTime())) {
          return date;
        }
      }

      return null;
    }

    function archiveTimeLabel(summary, raw, id) {
      const date = archiveDate(summary, raw);

      if (!date) {
        return tr('Дата неизвестна');
      }

      return new Intl.DateTimeFormat(
        currentLanguage === 'en'
          ? 'en-US'
          : 'ru-RU',
        {
          day:'2-digit',
          month:'2-digit',
          year:'2-digit',
          hour:'2-digit',
          minute:'2-digit'
        }
      ).format(date);
    }

    function archiveDateLabel(summary, raw) {
      const date = archiveDate(summary, raw);

      if (!date) return '';

      return new Intl.DateTimeFormat(
        currentLanguage === 'en'
          ? 'en-US'
          : 'ru-RU',
        {
          day:'2-digit',
          month:'short',
          year:'numeric',
          hour:'2-digit',
          minute:'2-digit'
        }
      ).format(date);
    }

    function archiveProfile(summary) {
      const profile = String(
        summary?.profile ??
        summary?.shot_profile ??
        ''
      ).trim();

      return profile
        ? profileLabel(profile)
        : tr('Профиль не указан');
    }

    function archiveDrinkWeight(summary) {
      return archiveSummaryValue(
        summary,
        [
          'coffee.drink_weight_g',
          'coffee.beverage_weight_g',
          'drink_weight_g',
          'beverage_weight_g',
          'weight_g'
        ],
        0
      );
    }

    function archiveTargetWeight(summary) {
      return archiveSummaryValue(
        summary,
        [
          'coffee.target_weight_g',
          'target_weight_g',
          'brew_target_weight_g'
        ],
        0
      );
    }

    function archiveDose(summary) {
      return archiveSummaryValue(
        summary,
        [
          'coffee.dose_g',
          'coffee.coffee_dose_g',
          'dose_g',
          'coffee_dose_g'
        ],
        0
      );
    }

    function archiveDuration(summary) {
      return archiveSummaryValue(
        summary,
        [
          'shot_duration_s',
          'duration_s',
          'brew_duration_s'
        ],
        0
      );
    }

    function archivePeakPressure(summary) {
      return archiveSummaryValue(
        summary,
        [
          'pressure.maximum_bar',
          'pressure.peak_bar',
          'maximum_pressure_bar',
          'peak_pressure_bar'
        ],
        0
      );
    }

    function archiveErrors(summary) {
      return archiveSummaryValue(
        summary,
        [
          'sensor.shot_errors',
          'shot_errors',
          'sensor_errors'
        ],
        0
      );
    }

    const SHOT_QUALITY_COPY = {
      ru: {
        quality: {
          good:'Хороший пролив',
          acceptable:'Есть небольшие отклонения',
          poor:'Требует корректировки',
          sensor_fault:'Проверить датчик',
          no_data:'Недостаточно данных'
        },
        diagnosis: {
          normal:'Давление и выход напитка соответствуют профилю.',
          overpressure:'Давление заметно превышало заданный профиль.',
          underpressure:'Среднее давление было ниже заданного профиля.',
          too_restrictive:'Напиток набирался медленно: таблетка слишком плотная.',
          too_free:'Целевой вес набран слишком быстро: пролив слишком свободный.',
          channeling_suspected:'Давление и поток нестабильны: возможно образование каналов.',
          unstable:'Давление заметно колебалось во время основной фазы.',
          sensor_fault:'Данные XDB401 недостаточно надёжны для оценки пролива.',
          no_data:'Запишите новый диагностический пролив для автоматической оценки.'
        },
        grind: {
          none:'Без изменений',
          finer:'Немного мельче',
          coarser:'Немного крупнее',
          check_puck:'Проверить распределение таблетки',
          unknown:'Нет рекомендации'
        }
      },
      en: {
        quality: {
          good:'Good shot',
          acceptable:'Minor deviations',
          poor:'Adjustment recommended',
          sensor_fault:'Check the sensor',
          no_data:'Not enough data'
        },
        diagnosis: {
          normal:'Pressure and beverage output match the selected profile.',
          overpressure:'Pressure rose noticeably above the requested profile.',
          underpressure:'Average pressure stayed below the requested profile.',
          too_restrictive:'The beverage accumulated slowly: the puck was too restrictive.',
          too_free:'Target weight was reached too quickly: the shot ran too freely.',
          channeling_suspected:'Pressure and flow were unstable; channeling may have occurred.',
          unstable:'Pressure fluctuated noticeably during the main extraction.',
          sensor_fault:'XDB401 data is not reliable enough to assess this shot.',
          no_data:'Record a new diagnostic shot to enable automatic assessment.'
        },
        grind: {
          none:'No change',
          finer:'Slightly finer',
          coarser:'Slightly coarser',
          check_puck:'Check puck preparation',
          unknown:'No recommendation'
        }
      }
    };

    function shotQualityText(group, code) {
      const language = SHOT_QUALITY_COPY[currentLanguage] || SHOT_QUALITY_COPY.ru;
      return language[group]?.[code] || SHOT_QUALITY_COPY.ru[group]?.[code] || code;
    }

    function archiveAnalysis(summary) {
      const analysis = summary?.analysis;
      return analysis && Number(analysis.version) >= 1 ? analysis : null;
    }

    function archiveQualityBadge(summary) {
      const analysis = archiveAnalysis(summary);
      if (!analysis) return '';
      const quality = String(analysis.quality || 'no_data');
      const score = Number(analysis.quality_score);
      const label = quality === 'sensor_fault'
        ? '!'
        : Number.isFinite(score)
          ? String(Math.round(score))
          : '—';
      return '<span class="archive-row-quality ' +
        archiveEscapeHtml(quality) + '" title="' +
        archiveEscapeHtml(shotQualityText('quality', quality)) + '">' +
        archiveEscapeHtml(label) + '</span>';
    }

    function renderShotQuality(summary) {
      const card = $('shotQualityCard');
      if (!card) return;
      const analysis = archiveAnalysis(summary);
      if (!analysis) {
        card.hidden = true;
        return;
      }

      const quality = String(analysis.quality || 'no_data');
      const diagnosis = String(analysis.diagnosis || 'no_data');
      const grind = String(analysis.suggested_grind || 'unknown');
      const score = Number(analysis.quality_score);
      const confidence = Number(analysis.sensor_confidence);
      const metrics = analysis.metrics || {};

      card.hidden = false;
      card.className = 'shot-quality-card ' + quality;
      $('shotQualityScore').textContent = Number.isFinite(score)
        ? Math.round(score) + '%'
        : '—';
      $('shotQualityTitle').textContent = shotQualityText('quality', quality);
      $('shotQualityDiagnosis').textContent = shotQualityText('diagnosis', diagnosis);
      $('shotQualityGrind').textContent = shotQualityText('grind', grind);
      $('shotQualityConfidence').textContent = Number.isFinite(confidence)
        ? Math.round(confidence) + '%'
        : '—';

      const pressureError = Number(metrics.mean_absolute_error_bar);
      $('shotQualityPressureError').textContent = Number.isFinite(pressureError)
        ? fmt(pressureError, 2) + ' ' + tr('бар')
        : '—';
      const averageFlow = Number(metrics.average_flow_g_s);
      $('shotQualityFlow').textContent = Number.isFinite(averageFlow)
        ? fmt(averageFlow, 2) + ' ' + tr('г/с')
        : '—';

      const summaryKey = String(summary?.id ?? summary?.timestamp ?? 'latest');
      if (card.dataset.summaryKey !== summaryKey) {
        card.dataset.summaryKey = summaryKey;
        const details = $('shotQualityDetails');
        const toggle = $('shotQualityToggle');
        if (details) details.hidden = true;
        if (toggle) {
          toggle.setAttribute('aria-expanded', 'false');
        }
      }
      const details = $('shotQualityDetails');
      const toggle = $('shotQualityToggle');
      if (toggle) {
        toggle.textContent = tr(details?.hidden ? 'Подробнее' : 'Скрыть');
      }
    }

    function toggleShotQualityDetails() {
      const details = $('shotQualityDetails');
      const toggle = $('shotQualityToggle');
      if (!details || !toggle) return;
      details.hidden = !details.hidden;
      toggle.setAttribute('aria-expanded', String(!details.hidden));
      toggle.textContent = tr(details.hidden ? 'Подробнее' : 'Скрыть');
    }

    async function refreshLatestShotAnalysis() {
      try {
        const response = await fetch('/shots/index.json', { cache:'no-store' });
        if (!response.ok) return false;
        const data = await response.json();
        const shots = Array.isArray(data.shots) ? data.shots : [];
        const raw = shots[shots.length - 1];
        const id = archiveShotId(raw);
        if (!id) return false;
        archiveSummaryCache.delete(id);
        const summary = await fetchArchiveSummary(id, raw);
        latestShotSummary = summary;
        if (homeShotRailMode !== 'shots') {
          renderShotQuality(shotCompletedThisSession ? summary : null);
        }
        return !!archiveAnalysis(summary);
      } catch (_) {
        return false;
      }
    }

    function archiveRowMeta(summary) {
      if (!summary) {
        return tr('Загрузка…');
      }

      const duration = archiveDuration(summary);
      const drink = archiveDrinkWeight(summary);
      const parts = [];

      if (drink > 0) {
        parts.push(
          tr('Вес') + ' ' +
          fmt(drink, 1) + ' ' + tr('г')
        );
      }

      if (duration > 0) {
        parts.push(
          tr('Время') + ' ' +
          fmt(duration, 1) + ' ' + tr('с')
        );
      }

      return parts.length
        ? parts.join(' · ')
        : tr('Данные пролива');
    }

    async function fetchArchiveSummary(
      id,
      raw = null
    ) {
      if (archiveSummaryCache.has(id)) {
        return archiveSummaryCache.get(id);
      }

      const rawLooksComplete =
        raw &&
        typeof raw === 'object' &&
        (
          raw.profile ||
          raw.shot_duration_s ||
          raw.coffee ||
          raw.pressure
        );

      if (rawLooksComplete) {
        archiveSummaryCache.set(id, raw);
        return raw;
      }

      const response = await fetch(
        '/shots/file/' +
        archiveStem(id) +
        '.json',
        { cache:'no-store' }
      );

      if (!response.ok) {
        throw Error('archive summary unavailable');
      }

      const summary = await response.json();
      archiveSummaryCache.set(id, summary);
      return summary;
    }

    async function fetchArchiveSeries(id) {
      if (archiveSeriesCache.has(id)) {
        return archiveSeriesCache.get(id);
      }

      const response = await fetch(
        '/shots/file/' +
        archiveStem(id) +
        '.csv',
        { cache:'no-store' }
      );

      if (!response.ok) {
        throw Error('archive csv unavailable');
      }

      const csv = await response.text();
      const lines = csv.trim().split(/\r?\n/);

      if (lines.length < 2) {
        throw Error('archive csv empty');
      }

      const head = lines.shift().split(',');
      const at = name => head.indexOf(name);
      const elapsedIndex = at('elapsed_ms');

      const series = lines.map(
        (line, index) => {
          const columns = line.split(',');

          return {
            x:
              elapsedIndex >= 0
                ? (
                    Number(
                      columns[elapsedIndex]
                    ) || 0
                  ) / 1000
                : index * .2,
            p:
              Number(
                columns[
                  at('pressure_bar')
                ]
              ) || 0,
            t:
              Number(
                columns[
                  at('target_bar')
                ]
              ) || 0,
            f:
              Number(
                columns[
                  at('flow_g_s')
                ]
              ) || 0,
            u:
              Number(
                columns[
                  at(
                    'final_output_percent'
                  )
                ]
              ) || 0
          };
        }
      );

      archiveSeriesCache.set(id, series);
      return series;
    }

    async function deleteSelectedArchiveShot(shotId = archiveSelectedId) {
      const id = Number(shotId);
      if (!id) return;

      const item = archiveIndex.find(entry => entry.id === id);
      const summary = archiveSummaryCache.get(id) || item?.summary || null;
      const label = archiveDateLabel(summary, item?.raw) || tr('без даты');

      if (!confirm(tr('Удалить сохранённый пролив') + ' (' + label + ')?')) {
        return;
      }

      try {
        const response = await fetch('/shots/delete/' + id, {
          method:'POST',
          cache:'no-store'
        });
        if (!response.ok) throw Error('archive delete failed');

        archiveSummaryCache.delete(id);
        archiveSeriesCache.delete(id);
        if (archiveSelectedId === id) {
          archiveSelectedId = 0;
          archiveSelectedSummary = null;
          archiveDetailSeries = [];
          archiveSeries = [];
          chartMode = 'auto';
          archiveTitle = '';
        }
        toast(tr('Пролив удалён'));
        await loadArchiveList(true);
        updateChartControls();
        draw();
      } catch (_) {
        toast(tr('Не удалось удалить пролив'), true);
      }
    }

    const ARCHIVE_SUMMARY_PREFETCH_LIMIT = 12;

    async function enrichArchiveListSummaries(
      token
    ) {
      for (const item of archiveIndex.slice(0, ARCHIVE_SUMMARY_PREFETCH_LIMIT)) {
        if (token !== archiveListLoadToken) {
          return;
        }

        if (
          archiveSummaryCache.has(
            item.id
          )
        ) {
          continue;
        }

        try {
          await fetchArchiveSummary(
            item.id,
            item.raw
          );
          renderHomeArchiveList();
        } catch (_) {}

        await sleep(25);
      }
    }

    function syncHomeShotRailUi() {
      const profilesTab = $('homeProfilesTab');
      const shotsTab = $('homeShotsTab');
      const profilesPane = $('homeProfilesPane');
      const shotsPane = $('homeShotsPane');
      const chartCard = $('shotChartCard');
      const archiveSummary = $('homeArchiveSummary');
      const profileSummary = $('shotProfileSummary');

      const shotsMode =
        homeShotRailMode === 'shots';

      profilesTab?.classList.toggle(
        'active',
        !shotsMode
      );
      shotsTab?.classList.toggle(
        'active',
        shotsMode
      );

      profilesTab?.setAttribute(
        'aria-selected',
        String(!shotsMode)
      );
      shotsTab?.setAttribute(
        'aria-selected',
        String(shotsMode)
      );

      if (profilesPane) {
        profilesPane.hidden = shotsMode;
      }

      if (shotsPane) {
        shotsPane.hidden = !shotsMode;
      }

      chartCard?.classList.toggle(
        'archive-view',
        shotsMode
      );

      if (archiveSummary) {
        archiveSummary.hidden = !shotsMode;
      }

      if (profileSummary) {
        profileSummary.hidden = shotsMode;
      }

      if (!shotsMode) {
        chartMode = 'auto';
        archiveTitle = '';
      }

      renderShotQuality(
        shotsMode
          ? archiveSelectedSummary
          : shotCompletedThisSession
            ? latestShotSummary
            : null
      );

      updateChartControls();
      updateMainCurveEditor();
      requestAnimationFrame(draw);
    }

            function initHomeShotRail() {
      const board = $('shotProfileBoard');

      try {
        const saved = localStorage.getItem(
          HOME_SHOT_RAIL_MODE_KEY
        );

        if (
          saved === 'profiles' ||
          saved === 'shots'
        ) {
          homeShotRailMode = saved;
        }
      } catch (_) {}

      if (
        board &&
        board.dataset.panelDragReady !== '1'
      ) {
        board.dataset.panelDragReady = '1';
        board.dataset.ready = '1';

        ensureShotProfilePanelHandles();
        ensureShotProfileResizeHandle();
        loadShotProfilePanelOrder();
        loadShotProfilePanelSize();

        document.addEventListener(
          'pointermove',
          moveShotPanelDrag,
          { passive:false }
        );
        document.addEventListener(
          'pointerup',
          finishShotPanelDrag
        );
        document.addEventListener(
          'pointercancel',
          finishShotPanelDrag
        );

        document.addEventListener(
          'pointermove',
          moveShotProfileResize,
          { passive:false }
        );
        document.addEventListener(
          'pointerup',
          finishShotProfileResize
        );
        document.addEventListener(
          'pointercancel',
          finishShotProfileResize
        );

        window.addEventListener(
          'resize',
          syncShotProfileSizeToViewport
        );
      }

      updateShotPanelHandleLabels();
      updateShotProfileResizeLabel();
      syncHomeShotRailUi();
      renderHomeArchiveList();
      void refreshLatestShotAnalysis();
    }

    function setHomeShotRailMode(
      mode,
      persist = true
    ) {
      homeShotRailMode =
        mode === 'shots'
          ? 'shots'
          : 'profiles';

      if (persist) {
        try {
          localStorage.setItem(
            HOME_SHOT_RAIL_MODE_KEY,
            homeShotRailMode
          );
        } catch (_) {}
      }

      syncHomeShotRailUi();

      if (homeShotRailMode === 'shots') {
        if (!archiveIndex.length) {
          void loadArchiveList();
        } else {
          const id =
            archiveSelectedId ||
            archiveIndex[0]?.id;

          if (id) {
            void selectHomeArchiveShot(id);
          }
        }
      }
    }

    function renderHomeArchiveList() {
      const list = $('homeArchiveList');
      const count = $('homeArchiveCount');
      const status = $('homeArchiveStatus');

      if (!list || !count) return;

      count.textContent = String(
        archiveIndex.length
      );

      if (status) {
        status.textContent =
          archiveIndex.length
            ? message(
                'localSaved',
                archiveIndex.length
              )
            : tr('Выберите запись');
      }

      if (!archiveIndex.length) {
        list.innerHTML =
          '<div class="history-list-state">' +
          archiveEscapeHtml(
            tr('История пока пуста')
          ) +
          '<br><small>' +
          archiveEscapeHtml(
            tr(
              'После первого пролива здесь появится запись.'
            )
          ) +
          '</small></div>';
        return;
      }

      list.innerHTML = archiveIndex
        .map(item => {
          const id = item.id;
          const summary =
            archiveSummaryCache.get(id) ||
            item.summary ||
            null;
          const active =
            homeShotRailMode === 'shots' &&
            chartMode === 'archive' &&
            id === archiveSelectedId;

          return (
            '<div class="archive-row-card' +
            (active ? ' active' : '') +
            '">' +
              '<button class="archive-row-open" type="button" onclick="selectHomeArchiveShot(' + id + ')">' +
                archiveQualityBadge(summary) +
                '<b class="archive-row-profile">' +
                  archiveEscapeHtml(archiveProfile(summary)) +
                '</b>' +
                '<span class="archive-row-metrics">' +
                  archiveEscapeHtml(
                    archiveRowMeta(summary)
                  ) +
                '</span>' +
                '<span class="archive-row-date">' +
                archiveEscapeHtml(
                  archiveTimeLabel(
                    summary,
                    item.raw,
                    id
                  )
                ) +
                '</span>' +
              '</button>' +
              '<button class="archive-row-delete" type="button" onclick="deleteSelectedArchiveShot(' + id + ')"' +
                (shotRunning ? ' disabled' : '') +
                ' aria-label="' + archiveEscapeHtml(tr('Удалить пролив')) +
                '" title="' + archiveEscapeHtml(tr('Удалить пролив')) + '">🗑</button>' +
            '</div>'
          );
        })
        .join('');
    }

    function renderHomeArchiveSummary(
      summary
    ) {
      if (!summary) return;

      const drink =
        archiveDrinkWeight(summary);
      const duration =
        archiveDuration(summary);
      const peak =
        archivePeakPressure(summary);
      const target =
        archiveTargetWeight(summary);

      $('homeArchiveDrinkWeight').textContent =
        drink > 0
          ? fmt(drink, 1) +
            ' ' +
            tr('г')
          : '—';

      $('homeArchiveDuration').textContent =
        duration > 0
          ? fmt(duration, 1) +
            ' ' +
            tr('с')
          : '—';

      $('homeArchivePeakPressure').textContent =
        peak > 0
          ? fmt(peak, 2) +
            ' ' +
            tr('бар')
          : '—';

      $('homeArchiveTargetWeight').textContent =
        target > 0
          ? fmt(target, 1) +
            ' ' +
            tr('г')
          : '—';

      if (homeShotRailMode === 'shots') {
        renderShotQuality(summary);
      }
    }

    async function selectHomeArchiveShot(id) {
      id = Number(id);

      if (!id) return;

      homeShotRailMode = 'shots';
      archiveSelectedId = id;
      syncHomeShotRailUi();
      renderHomeArchiveList();

      const token = ++homeArchiveLoadToken;
      const item = archiveIndex.find(
        entry => entry.id === id
      );
      const status = $('homeArchiveStatus');

      if (status) {
        status.textContent =
          tr('Загрузка пролива…');
      }

      try {
        const [summary, series] =
          await Promise.all([
            fetchArchiveSummary(
              id,
              item?.raw
            ),
            fetchArchiveSeries(id)
          ]);

        if (token !== homeArchiveLoadToken) {
          return;
        }

        archiveSelectedSummary = summary;
        archiveDetailSeries = series;
        archiveSeries = series.map(
          point => ({ ...point })
        );
        chartMode = 'archive';
        archiveTitle =
          (archiveDateLabel(summary, item?.raw) || tr('Пролив без даты')) +
          ' · ' + archiveProfile(summary);

        renderHomeArchiveSummary(summary);
        renderHomeArchiveList();

        if (status) {
          status.textContent =
            archiveProfile(summary);
        }

        updateChartControls();
        updateMainCurveEditor();
        draw();
      } catch (_) {
        if (token !== homeArchiveLoadToken) {
          return;
        }

        if (status) {
          status.textContent =
            tr('Не удалось открыть пролив');
        }

        toast(
          tr('Не удалось открыть пролив'),
          true
        );
      }
    }

    async function loadArchiveList(
      force = false
    ) {
      const token = ++archiveListLoadToken;
      const list = $('homeArchiveList');

      if (list) {
        list.innerHTML =
          '<div class="history-list-state">' +
          archiveEscapeHtml(
            tr('Загрузка истории…')
          ) +
          '</div>';
      }

      try {
        let shots = [];

        const response = await fetch(
          '/shots/index.json',
          { cache:'no-store' }
        );

        if (!response.ok) {
          throw Error(
            'archive index unavailable'
          );
        }

        const data = await response.json();
        shots = Array.isArray(data.shots)
          ? [...data.shots].reverse()
          : [];

        if (token !== archiveListLoadToken) {
          return;
        }

        if (force) {
          archiveSummaryCache.clear();
          archiveSeriesCache.clear();
        }

        archiveIndex = shots
          .map(raw => ({
            id:archiveShotId(raw),
            raw,
            summary:
              raw &&
              typeof raw === 'object'
                ? raw
                : null
          }))
          .filter(item => item.id);

        if (
          !archiveIndex.some(
            item =>
              item.id ===
              archiveSelectedId
          )
        ) {
          archiveSelectedId =
            archiveIndex[0]?.id || 0;
        }

        renderHomeArchiveList();

        if (!archiveSelectedId) {
          archiveSelectedSummary = null;
          archiveDetailSeries = [];
          return;
        }

        if (homeShotRailMode === 'shots') {
          await selectHomeArchiveShot(
            archiveSelectedId
          );
        }

        // Fill the first visible rows immediately, even while Home remains
        // in profile mode. The rest of the archive stays lazy-loaded.
        void enrichArchiveListSummaries(
          token
        );
      } catch (_) {
        archiveIndex = [];
        archiveSelectedId = 0;
        archiveSelectedSummary = null;
        archiveDetailSeries = [];

        if (list) {
          list.innerHTML =
            '<div class="history-list-state">' +
            archiveEscapeHtml(
              tr(
                'Архив пока недоступен'
              )
            ) +
            '</div>';
        }

      }
    }

    async function loadHomeStatus(){
      const items=[
        ['switch','Silvia Hot Water',v=>setHotWater(stateOn(v.value??v.state))],['switch','Silvia Steam Mode',v=>setSteamMode(stateOn(v.value??v.state))],['binary_sensor','Silvia Water Level',v=>showWaterLevel(stateOn(v.value??v.state))],
        ['sensor','Silvia Backflush Shots',v=>values.backflushShots=Number(v.value)],['sensor','Silvia Monthly Shots',v=>values.monthlyShots=Number(v.value)],['sensor','Silvia Monthly Drink Weight',v=>values.monthlyDrinkWeight=Number(v.value)],['sensor','Silvia Auto Off Remaining',v=>values.autoOffRemaining=Number(v.value)],['number','Silvia Backflush Reminder Shots',v=>values.backflushReminderLive=Number(v.value)],['number','Silvia Steam Target',v=>values.steamTargetLive=Number(v.value)],['switch','Silvia Stop Brew By Weight',v=>{const node=$('stopByWeight');if(node)node.checked=stateOn(v.value??v.state);updateStopByWeightControl();}]
      ];
      await Promise.all(items.map(async([d,n,a])=>{try{a(await get(d,n));}catch(_){}})); updateHomeDashboard(); updateQuickActions();
    }

    async function syncState() {
      try {
        const [profile, temp, target, heat, brew, power, weight] = await Promise.all([
          get('select', 'Silvia Shot Profile'),
          get('sensor', 'Silvia Estimated Brew Temperature'),
          get('number', 'Silvia Brew Target'),
          get('sensor', 'Silvia PID Heat Output'),
          get('switch', 'Silvia Brew Shot'),
          get('switch', 'Silvia Power Relay'),
          get('sensor', 'Silvia Weight')
        ]);

        const nextProfile =
          String(profile.value ?? profile.state ?? '');
        const profileChanged =
          !!nextProfile &&
          nextProfile !== currentProfileName;

        currentProfileName = nextProfile || currentProfileName;
        syncProfileControls(currentProfileName);
        updateShotScenario();

        values.temp = Number(temp.value ?? temp.state) || 0;
        values.tempTarget = Number(target.value ?? target.state) || 0;
        values.heat = Number(heat.value ?? heat.state) || 0;
        values.weight = Number(weight.value ?? weight.state);
        observeShotWeight(values.weight);

        if ($('temp')) $('temp').textContent = fmt(values.temp);
        if ($('tempTarget')) $('tempTarget').textContent = fmt(values.tempTarget);

        updateDiagnosticMirrors('temp', values.temp);
        updateDiagnosticMirrors('heat', values.heat);
        setShotRunning(
          stateOn(brew.value ?? brew.state)
        );
        showPower(
          stateOn(power.value ?? power.state)
        );

        updateHomeDashboard();
        updateQuickActions();
        sample();

        if (profileChanged && !shotRunning) {
          await loadHomeProfileSnapshot(true);
        }
      } catch (_) {}
    }

    async function fallbackSyncTick() {
      if (
        eventStreamHealthy() ||
        fallbackSyncBusy
      ) {
        return;
      }

      fallbackSyncBusy = true;

      try {
        await Promise.all([
          syncState(),
          loadHomeStatus()
        ]);

        const active =
          document.querySelector('.page.active');

        if (active?.id === 'page-settings') {
          await refreshStatus(SETTINGS_STATUS_IDS);
        } else if (active?.id === 'page-maintenance') {
          await refreshStatus(MAINTENANCE_STATUS_IDS);
        } else if (active?.id === 'page-diagnostics') {
          await refreshStatus();
        }
      } finally {
        fallbackSyncBusy = false;
      }
    }

    window.addEventListener('resize', () => {
      draw();
      drawTemp();
      drawRecipePreview();
      updateMainCurveEditor();
    });
    initHomeValueScrub();
    const mainChart = $('chart');
    mainChart.addEventListener('dblclick', handleMainChartDoubleClick);
    mainChart.addEventListener('pointerdown', handleMainChartPointerDown);
    mainChart.addEventListener('pointermove', handleMainChartPointerMove);
    mainChart.addEventListener('pointerup', finishMainCurvePointer);
    mainChart.addEventListener('pointercancel', event => {
      finishMainCurvePointer(event);
      chartHoverIndex = -1;
      hideChartTooltip();
      draw();
    });
    mainChart.addEventListener('pointerleave', event => {
      if (!mainCurveDragKey) {
        mainCurveHoverKey = '';
        chartHoverIndex = -1;
        hideChartTooltip();
        draw();
      }
    });
    document.addEventListener('keydown', event => {
      if (event.key === 'Escape' && $('shotChartCard')?.classList.contains('expanded')) toggleChartExpanded(false);
    });

    const recipeChart = $('recipeChart');
    if (recipeChart) {
      recipeChart.addEventListener('pointerdown', handleRecipePointerDown);
      recipeChart.addEventListener('pointermove', handleRecipePointerMove);
      recipeChart.addEventListener('pointerup', finishRecipePointer);
      recipeChart.addEventListener('pointercancel', finishRecipePointer);
      recipeChart.addEventListener('pointerleave', event => {
        if (!recipeDragKey) {
          recipeHoverKey = '';
          drawRecipePreview();
        }
      });
    }

    document.querySelectorAll('.nav-item').forEach(button => button.addEventListener('click', () => showPage(button.dataset.page)));

    /*
     * Desktop navigation always starts collapsed. On mobile the drawer
     * remains closed until the menu button is pressed.
     */
    document.body.classList.remove('nav-open');
    if (!matchMedia('(max-width:840px)').matches) {
      document.body.classList.add('sidebar-collapsed');
    }

    initLanguage();
    initSettingsWorkspace();
    initSettingsHelp();
    initServiceWorkspaces();
    initMaintenanceHelp();
    loadTypographySettings();
    loadShotLineVisibility();
    loadShotLineSettings();
    loadUserProfiles({ silent:true });
    initHomeDashboardLayout();
    initHomeInfoCardOrder();
    connectionLastOkAt = Date.now();
    scheduleDashboardOffline();

    let events = null;

    events = new EventSource('/events');

    events.addEventListener('open', () => {
      markEventStreamActivity();
    });

    events.addEventListener('error', () => {
      markEventStreamError();
    });

    events.addEventListener('ping', () => {
      markEventStreamActivity();
    });

    events.addEventListener('state', event => {
      markEventStreamActivity();
      try {
        apply(JSON.parse(event.data));
      } catch (_) {}
    });

    /*
     * Native ESPHome startup model: EventSource('/events') supplies the
     * initial DETAIL_ALL entity dump and all later state changes. No REST
     * profile snapshot is issued on page load; profile Number events are
     * coalesced into one graph redraw by scheduleProfileStateCommitFromSse().
     */
    loadArchiveList();


    setTemperatureWindow(600);
    setHomeTemperatureMode('state');
    updateShotScenario();
    updateMainCurveEditor();

    /*
     * REST is fallback only. It does not compete with healthy SSE traffic,
     * so the ESP32 avoids continuous duplicate polling.
     */
    setTimeout(fallbackSyncTick, 2500);
    setInterval(
      fallbackSyncTick,
      fallbackSyncIntervalMs
    );

    setInterval(
      updateShotPhaseStatus,
      1000
    );
  </script>


</body>

</html>

)HTML";

class CoffeeDashboardHandler : public AsyncWebHandler {
 public:
  bool canHandle(AsyncWebServerRequest *request) const override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);

    if (request->method() == HTTP_GET)
      return url == "/coffee" || url == "/coffee/";

    if (request->method() == HTTP_POST)
      return url == "/custom-phases/apply" ||
             url == "/custom-phases/clear";

    return false;
  }

  void handleRequest(AsyncWebServerRequest *request) override {
    char buffer[AsyncWebServerRequest::URL_BUF_SIZE];
    const std::string url = request->url_to(buffer);

    if (request->method() == HTTP_POST) {
      if (url == "/custom-phases/clear") {
        silvia::clear_custom_shot_phases();
        auto *response = request->beginResponse(
            200,
            "application/json; charset=utf-8",
            "{\"ok\":true}");
        response->addHeader("Cache-Control","no-store");
        request->send(response);
        return;
      }

      if (url == "/custom-phases/apply") {
        if (!request->hasParam("spec")) {
          request->send(400,"application/json","{\"ok\":false,\"error\":\"missing_spec\"}");
          return;
        }

        std::vector<silvia::ShotPhase> phases;
        std::string error;
        const std::string spec = request->getParam("spec")->value();

        if (!silvia_profiles::parse_phase_spec(spec,phases,&error) ||
            !silvia::set_custom_shot_phases(phases,&error)) {
          const std::string body =
              std::string("{\"ok\":false,\"error\":\"") +
              silvia_profiles::json_escape(error.empty() ? "invalid_phases" : error) +
              "\"}";
          request->send(400,"application/json",body.c_str());
          return;
        }

        auto *response = request->beginResponse(
            200,
            "application/json; charset=utf-8",
            "{\"ok\":true}");
        response->addHeader("Cache-Control","no-store");
        request->send(response);
        return;
      }
    }

    auto *response = request->beginResponse(
        200,
        "text/html; charset=utf-8",
        reinterpret_cast<const uint8_t *>(COFFEE_DASHBOARD_HTML),
        sizeof(COFFEE_DASHBOARD_HTML) - 1);
    response->addHeader("Cache-Control", "no-store");
    response->addHeader("X-Content-Type-Options", "nosniff");
    request->send(response);
  }
};

inline CoffeeDashboardHandler coffee_dashboard_handler;
inline bool handler_registered = false;

inline void register_web_handler() {
  if (handler_registered || esphome::web_server_base::global_web_server_base == nullptr)
    return;
  esphome::web_server_base::global_web_server_base->add_handler(&coffee_dashboard_handler);
  handler_registered = true;
}

}  // namespace silvia_web
